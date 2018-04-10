// Created by Indeximal 09.04.2018
#pragma once

#include "IndeximalGameEngine.h"

#include <fstream>

namespace ige {
	class Shader {
	protected:
		GLuint shaderProgram;
		GLuint vertexShader;
		GLuint fragmentShader;
		bool wasLinked = false;

		GLuint loadShader(std::string path, GLenum type) {
			std::ifstream file(path);
			if (!file.good())
				throw std::logic_error("Shader file wasn't found: " + path);
			std::string source((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
			
			GLuint shaderID = glCreateShader(type);
			GLint len = (GLint) source.length();
			GLchar *data = (GLchar*) source.data();
			glShaderSource(shaderID, 1, &data, &len);
			glCompileShader(shaderID);
			GLint success;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
			if (success == GL_FALSE) {
				GLint errorLen;
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &errorLen);
				GLchar *errorBuf = new GLchar[errorLen];
				glGetShaderInfoLog(shaderID, errorLen, nullptr, errorBuf);
				std::string errorMsg(errorBuf);
				delete[] errorBuf;
				throw std::logic_error("Shader coudn't be compiled: " + errorMsg);
			}

			return shaderID;
		}

		void linkProgram() {
			if (!wasLinked) {
				glLinkProgram(shaderProgram);
				GLint result;
				glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
				if (result == GL_FALSE)
					throw std::logic_error("Shader coundn't be linked");
				wasLinked = true;
			}
		}

	public:
		Shader(const std::string vertexFile, const std::string fragmentFile) {
			vertexShader = loadShader(vertexFile, GL_VERTEX_SHADER);
			fragmentShader = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
			shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);
		}

		void start() {
			if (!wasLinked) {
				logWarning("Shader hasn't been linked yet!");
				linkProgram();
			}
			glUseProgram(shaderProgram);
		}

		void stop() {
			glUseProgram(0);
		}

		~Shader() {
			stop();
			glDetachShader(shaderProgram, fragmentShader);
			glDetachShader(shaderProgram, vertexShader);
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);
			glDeleteProgram(shaderProgram);
		}

		Shader(const Shader& other) = delete;
		Shader(Shader&& other) = delete;
		Shader& operator=(const Shader& other) = delete;
		Shader& operator=(Shader&& other) = delete;
	};

	class StaticShader : public Shader {
	public:
		StaticShader(): Shader("DefaultShader.vert", "DefaultShader.frag") {
			glBindAttribLocation(shaderProgram, 0, "position");
			linkProgram();
		}
	};

}