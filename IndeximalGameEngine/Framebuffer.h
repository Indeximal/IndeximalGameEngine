// Created by Indeximal 17.04.2018
#pragma once

#include "IndeximalGameEngine.h"

namespace ige {
	class Framebuffer {
	protected:
		GLuint fbo;
		std::vector<GLuint> attachments;

		int width;
		int height;

		float bgR = 1.0f, bgG = 1.0f, bgB = 1.0f, bgA = 1.0f;

	public:
		Framebuffer(int width, int height) 
			: width(width), height(height)
		{
			glGenFramebuffers(1, &fbo);
		}

		void attachColorTexture(Texture& textureBuffer, unsigned int attachmentNumber) {
			attachments.push_back(GL_COLOR_ATTACHMENT0 + attachmentNumber);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentNumber, GL_TEXTURE_2D, textureBuffer.getNativeID(), 0);
		}

		void attachDepthTexture(Texture& depthTexture) {
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture.getNativeID(), 0);
		}

		void clear() {
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glClearColor(bgR, bgG, bgB, bgA);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void setClearColor(float r, float g, float b, float a = 1.0f) {
			bgR = r;
			bgG = g;
			bgB = b;
			bgA = a;
		}

		int getWidth() {
			return width;
		}

		int getHeight() {
			return height;
		}

		std::vector<GLuint>& getNativeAttachments() {
			return attachments;
		}

		bool checkStatusOK() {
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
		}

		GLuint getNativeFboID() {
			return fbo;
		}

		void destroy() {
			glDeleteFramebuffers(1, &fbo);
		}

		~Framebuffer() {
			destroy();
		}

		Framebuffer(const Framebuffer& other) = delete; // copy constructor
		Framebuffer(Framebuffer&& other) = default; // move constructor
		Framebuffer& operator=(const Framebuffer& other) = delete; // copy assignment
		Framebuffer& operator=(Framebuffer&& other) = delete; // move assignment
	};

	class GBuffer : public Framebuffer {
	private:
		Texture colorBuf;
		Texture normalBuf;
		Texture positionBuf;
		Texture depthBuf;
	public:
		GBuffer(int width, int height) : Framebuffer(width, height) {
			colorBuf.initEmpty(width, height, ige::FormatType::RGB, ige::FormatType::RGB, ige::DataType::UBYTE);
			attachColorTexture(colorBuf, 0);

			normalBuf.initEmpty(width, height, ige::FormatType::RGB16F, ige::FormatType::RGB, ige::DataType::FLOAT);
			attachColorTexture(normalBuf, 1);

			positionBuf.initEmpty(width, height, ige::FormatType::RGB16F, ige::FormatType::RGB, ige::DataType::FLOAT);
			attachColorTexture(positionBuf, 2);

			depthBuf.initEmpty(width, height, ige::FormatType::DEPTH, ige::FormatType::DEPTH, ige::DataType::FLOAT);
			attachDepthTexture(depthBuf);

			if (!checkStatusOK()) ige::logError("Failed to init GBuffer!");
		}

		void clear() {
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			float zeroArr[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			float bgArrF[4] = { bgR, bgG, bgB, 1.0f };
 			glClearBufferfv(GL_COLOR, 0, bgArrF);
 			glClearBufferfv(GL_COLOR, 1, zeroArr);
			glClearBufferfv(GL_COLOR, 2, zeroArr);
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		Texture& getColorBuffer() {
			return colorBuf;
		}

		Texture& getNormalBuffer() {
			return normalBuf;
		}

		Texture& getPositionBuffer() {
			return positionBuf;
		}

		Texture& getDepthBuffer() {
			return depthBuf;
		}
	};

	class GeometryPassShader : public ShaderProgram {
	public:
		UniformMat4 modelMatrix;
		UniformMat4 viewMatrix;
		UniformMat4 projectionMatrix;
		UniformSampler texture;

		GeometryPassShader()
			: ShaderProgram("StaticShader.vert", "GeometryPassShader.frag", { "position", "normal", "uv_coord" }),
			modelMatrix("modelMat", shaderProgram),
			viewMatrix("viewMat", shaderProgram),
			projectionMatrix("projMat", shaderProgram),
			texture("tex", shaderProgram, 0)
		{}
	};

	class Geometry2DShader : public ShaderProgram {
	public:
		UniformSampler texture;

		Geometry2DShader()
			: ShaderProgram("2DTextureShader.vert", "2DShaderDeferred.frag", { "position", "texCoords" }),
			texture("tex", shaderProgram, 0)
		{}
	};

	class LightingPassShader : public ShaderProgram {
	public:
		UniformVec3 lightDirection;
		UniformVec3 viewPosition;
		UniformSampler albedoTexture;
		UniformSampler normalTexture;
		UniformSampler positionTexture;

		LightingPassShader()
			: ShaderProgram("2DTextureShader.vert", "LightingPassShader.frag", { "position", "texCoords" }),
			lightDirection("lightDirection", shaderProgram),
			viewPosition("viewPosition", shaderProgram),
			albedoTexture("albedoBuffer", shaderProgram, 0),
			normalTexture("normalBuffer", shaderProgram, 1),
			positionTexture("positionBuffer", shaderProgram, 2)
		{}
	};
}

