// Created by Indeximal 09.04.2018

#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <iostream>
#include <string>
#include <vector>

#define IGE_VERSION_MAJOR 0
#define IGE_VERSION_MINOR 0
#define IGE_VERSION_PATCH 1

namespace ige {
	template <typename T>
	void debugLog(T msg) {
		std::cout << "(Debug) " << msg << std::endl;
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
}

#include "Display.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"