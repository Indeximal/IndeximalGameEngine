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
				logError(errorMsg);
				delete[] errorBuf;
				throw std::logic_error("Shader coudn't be compiled: " + errorMsg);
			}
			return shaderID;
		}

		void linkProgram() {
			glLinkProgram(shaderProgram);
			GLint result;
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
			if (result == GL_FALSE)
				throw std::logic_error("Shader coundn't be linked");
		}

	public:
		ShaderProgram(const std::string vertexFile, const std::string fragmentFile, const std::vector<std::string> attributeNames) {
			vertexShader = loadShader(vertexFile, GL_VERTEX_SHADER);
			fragmentShader = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
			shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);
			for (int i = 0; i < attributeNames.size(); i++) {
				glBindAttribLocation(shaderProgram, i, attributeNames[i].c_str());
			}
			linkProgram();
		}

		void start() {
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
		ShaderProgram(ShaderProgram&& other) = default;
		ShaderProgram& operator=(const ShaderProgram& other) = delete;
		ShaderProgram& operator=(ShaderProgram&& other) = delete;
	};

	class Uniform {
	protected:
		GLint location;
		GLuint program;	
		GLint prevProgram;

		void loadProgram() {
			glGetIntegerv(GL_CURRENT_PROGRAM, &prevProgram);
			if (prevProgram != program)
				glUseProgram(program);
		}
		void restoreProgram() {
			if (prevProgram != program)
				glUseProgram(prevProgram);
		}

		Uniform(std::string name, GLuint program) : program(program) {
			location = glGetUniformLocation(program, name.c_str());
			if (location == -1) logError("Uniform " + name + " not found!");
		}

		void set3f(float x, float y, float z) {
			loadProgram();
			glUniform3f(location, x, y, z);
			restoreProgram();
		}
	};

	class UniformSampler : Uniform {
	private:
		unsigned int number;
	public:
		UniformSampler(std::string name, GLuint program, int unsigned number) 
			: Uniform(name, program), number(number)
		{
			loadProgram();
			glUniform1i(location, number);
			restoreProgram();
		}

		void operator=(const Texture& texture) {
			glActiveTexture(GL_TEXTURE0 + number);
			glBindTexture(GL_TEXTURE_2D, texture.getNativeID());
			glActiveTexture(GL_TEXTURE0);
		}
	};

	class UniformMat4 : Uniform {
	public:
		UniformMat4(std::string name, GLuint program) : Uniform(name, program) {}
		void operator=(const glm::mat4& mat) {
			loadProgram();
			glUniformMatrix4fv(location, 1, GL_FALSE, (GLfloat*)&mat);
			restoreProgram();
		}
	};

	class UniformVec3 : Uniform {
	public:
		UniformVec3(std::string name, GLuint program) : Uniform(name, program) {}
		void operator=(const glm::vec3& vec) {
			loadProgram();
			glUniform3fv(location, 1, (GLfloat*)&vec);
			restoreProgram();
		}
	};

	class StaticShader : public ShaderProgram {
	public:
		UniformMat4 modelMatrix;
		UniformMat4 viewMatrix;
		UniformMat4 projectionMatrix;
		UniformVec3 lightDirection;
		UniformVec3 tint;
		UniformVec3 viewPosition;
		UniformSampler texture;
	
		StaticShader()
			: ShaderProgram("StaticShader.vert", "StaticShaderTexture.frag", { "position", "normal", "uv_coord" }),
			modelMatrix("modelMat", shaderProgram),
			viewMatrix("viewMat", shaderProgram),
			projectionMatrix("projMat", shaderProgram),
			lightDirection("lightDirection", shaderProgram),
			viewPosition("viewPosition", shaderProgram),
			tint("tint", shaderProgram),
			texture("tex", shaderProgram, 0)
		{}
	};

	class Shader2D : public ShaderProgram {
	public:
		UniformSampler texture;
		Shader2D()
			: ShaderProgram("2DTextureShader.vert", "2DTextureShader.frag", { "position", "texCoords" }),
			texture("tex", shaderProgram, 0)
		{}
	};

}