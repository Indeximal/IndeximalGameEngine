// Created by Indeximal 09.04.2018
#pragma once

#include "IndeximalGameEngine.h"

#include <fstream>

namespace ige {
	class ShaderProgram {
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
		ShaderProgram(const std::string vertexFile, const std::string fragmentFile) {
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

		~ShaderProgram() {
			stop();
			glDetachShader(shaderProgram, fragmentShader);
			glDetachShader(shaderProgram, vertexShader);
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);
			glDeleteProgram(shaderProgram);
		}

		ShaderProgram(const ShaderProgram& other) = delete;
		ShaderProgram(ShaderProgram&& other) = delete;
		ShaderProgram& operator=(const ShaderProgram& other) = delete;
		ShaderProgram& operator=(ShaderProgram&& other) = delete;
	};

	class StaticShader : public ShaderProgram {
	private:
		GLint uniformMVP;
		GLint uniformColor;
		GLint uniformLightDir;
	
	public:
		StaticShader(): ShaderProgram("DefaultShader.vert", "DefaultShader.frag") {
			glBindAttribLocation(shaderProgram, 0, "position");
			glBindAttribLocation(shaderProgram, 1, "normal");
			linkProgram();
			uniformMVP = glGetUniformLocation(shaderProgram, "MVPMatrix");
			uniformColor = glGetUniformLocation(shaderProgram, "tintColor");
			uniformLightDir = glGetUniformLocation(shaderProgram, "sunDirection");
		}

		void setMVP(const glm::mat4 &mat) {
			GLint currentProgram;
			glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
			glUseProgram(shaderProgram);
			glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, (GLfloat*) &mat);
			glUseProgram(currentProgram);
		}

		void setTintColor(float r, float g, float b) {
			GLint currentProgram;
			glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
			glUseProgram(shaderProgram);
			glUniform3f(uniformColor, r, g, b);
			glUseProgram(currentProgram);
		}

		void setLightDir(float x, float y, float z) {
			auto normal = glm::normalize(glm::vec3(x, y, z));
			GLint currentProgram;
			glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
			glUseProgram(shaderProgram);
			glUniform3f(uniformLightDir, normal.x, normal.y, normal.z);
			glUseProgram(currentProgram);
		}
	};

}