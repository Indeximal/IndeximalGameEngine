// Created by Indeximal 08.04.2018
#pragma once

#include "IndeximalGameEngine.h"

namespace ige {


	class Display {
	private:
		GLFWwindow * window;
		unsigned int width;
		unsigned int height;
		bool resizable;
		float bgR, bgG, bgB;

	public:
		Display(unsigned int width, unsigned int height, bool resizable, std::string title)
			: width(width), height(height), resizable(resizable), bgR(1.0f), bgG(1.0f), bgB(1.0f)
		{
			logInfo("IndeximalGameEngine v" + getIGEVersion());
			glfwInit();
			glfwWindowHint(GLFW_RESIZABLE, resizable);
			window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
			_makeContext();
		}

		void _makeContext() {
			glfwMakeContextCurrent(window);
			gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		}

		bool update() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glfwSwapBuffers(window);
			glfwPollEvents();
			//glEnable(GL_DEPTH_TEST);
			glClearColor(bgR, bgG, bgB, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			return !glfwWindowShouldClose(window);
		}

		void setBackgroundColor(float r, float g, float b) {
			bgR = r;
			bgG = g;
			bgB = b;
		}

		unsigned int getWidth() {
			return width;
		}

		unsigned int getHeight() {
			return height;
		}

		float getAspectRatio() {
			return (float)width / (float)height;
		}

		GLFWwindow* getNativeWindowPtr() {
			return window;
		}

		~Display() {
			glfwDestroyWindow(window);
		}

		Display(const Display& other) = delete; // copy constructor
		Display(Display&& other) = delete; // move constructor
		Display& operator=(const Display& other) = delete; // copy assignment
		Display& operator=(Display&& other) = delete; // move assignment
	};


}