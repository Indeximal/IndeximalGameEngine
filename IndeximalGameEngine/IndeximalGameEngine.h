// Created by Indeximal 09.04.2018

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "tiny_obj_loader.h"

#include <iostream>
#include <string>
#include <vector>

#define IGE_VERSION_MAJOR 0
#define IGE_VERSION_MINOR 0
#define IGE_VERSION_PATCH 1

namespace ige {
	const glm::vec3 forwardVector = { 1.0f, 0.0f, 0.0f };
	const glm::vec3 leftSidedVector = { 0.0f, 1.0f, 0.0f };
	const glm::vec3 upwardsVector = { 0.0f, 0.0f, 1.0f };

	template <typename T>
	void debugLog(T msg) {
		std::cout << "(Debug) " << msg << std::endl;
	}
	
	template <typename T>
	void debugLog(std::string name, T val) {
		std::cout << "(Debug) " << name << ": " << val << std::endl;
	}

	template <typename T>
	void logInfo(T msg) {
		std::cout << "(Info) " << msg << std::endl;
	}

	template <typename T>
	void logWarning(T msg) {
		std::cout << "(Warning) " << msg << std::endl;
	}

	template <typename T>
	void logError(T msg) {
		std::cout << "(Error) " << msg << std::endl;
	}

	std::string getOpenGLVersion() {
		return std::string((char*) glGetString(GL_VERSION));
	}

	std::string getIGEVersion() {
		return "" + std::to_string(IGE_VERSION_MAJOR) + "." + std::to_string(IGE_VERSION_MINOR) + "." + std::to_string(IGE_VERSION_PATCH);
	}

	double getTimeSinceStart() {
		return glfwGetTime();
	}

	long getRandomSeed() {
		return (long)(ige::getTimeSinceStart() * 100000.0);
	}
}

#include "Display.h"
#include "Camera.h" // dep: Display
#include "Model.h"
#include "Texture.h"
#include "Shader.h" // dep: Texture
#include "Terrain.h" // dep: Model, Shader
#include "Framebuffer.h" //dep: Texture, Shader
#include "Entity.h" // dep Model, Texture, Shader

#include "Renderer.h"