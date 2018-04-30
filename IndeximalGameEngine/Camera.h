// Created by Indeximal 4/20/2018 5:20:56 PM
#pragma once

#include "IndeximalGameEngine.h"
#include <glm/gtx/euler_angles.hpp>

namespace ige {

	class Camera {
	private:
		Display& display;

		glm::vec3 position;
		glm::vec3 viewDirection;
		glm::mat4 viewMatrix;
		glm::mat4 projMatrix;
		double prevX = 0.0;
		double prevY = 0.0;
		double angleDY = 0.0;
		double angleDX = 0.0;

		float speed = 5.0f;
		float sensitvity = 0.001f;

		void updateViewMatrix() {
			double cosY = glm::cos(angleDY);
			viewDirection = { glm::cos(angleDX) * cosY, glm::sin(angleDX) * cosY, glm::sin(angleDY) };
			viewMatrix = glm::lookAt(position, position + viewDirection, upwardsVector);
		}

	public:
		Camera(Display& display, const glm::vec3& eye, float fovDeg, float nearClipPlane, float farClipPlane) 
			: display(display), position(eye)
		{
			projMatrix = glm::perspective(glm::radians(fovDeg), display.getAspectRatio(), nearClipPlane, farClipPlane);
			updateViewMatrix();
		
			glfwSetInputMode(display.getNativeWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwGetCursorPos(display.getNativeWindowPtr(), &prevX, &prevY);
		}

		void setOrientation(float dx, float dy) {
			angleDX = dx;
			angleDY = dy;
			updateViewMatrix();
		}

		void update() {
			double x, y;
			glfwGetCursorPos(display.getNativeWindowPtr(), &x, &y);

			angleDX = std::fmod<double>(angleDX + (prevX - x) * sensitvity, glm::two_pi<double>());
			angleDY = glm::clamp(angleDY + (prevY - y) * sensitvity, -glm::pi<double>() / 2.0 + 0.0001, glm::pi<double>() / 2.0 - 0.0001);

			glm::vec3 velocity;
			if (glfwGetKey(display.getNativeWindowPtr(), GLFW_KEY_W) == GLFW_PRESS)
				velocity += viewDirection;

			if (glfwGetKey(display.getNativeWindowPtr(), GLFW_KEY_S) == GLFW_PRESS)
				velocity += -viewDirection;

			if (glfwGetKey(display.getNativeWindowPtr(), GLFW_KEY_A) == GLFW_PRESS)
				velocity += glm::normalize(glm::cross(upwardsVector, viewDirection));

			if (glfwGetKey(display.getNativeWindowPtr(), GLFW_KEY_D) == GLFW_PRESS)
				velocity += -glm::normalize(glm::cross(upwardsVector, viewDirection));

			if (velocity != glm::vec3(0.0f)) {
				velocity = glm::normalize(velocity);
				velocity *= (float)display.getFrameTime() * speed;
				position += velocity;
			}

			updateViewMatrix();

			prevX = x;
			prevY = y;
		}

		void setSpeed(float speedInUnitsPerSecond) {
			speed = speedInUnitsPerSecond;
		}

		glm::mat4 getViewMatrix() const {
			return viewMatrix;
		}

		glm::mat4 getProjectionMatrix() const {
			return projMatrix;
		}

		glm::vec3 getPosition() const {
			return position;
		}
	};

}