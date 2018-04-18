// Created by Indeximal 17.04.2018
#pragma once

#include "IndeximalGameEngine.h"

namespace ige {
	class Framebuffer {
		GLuint fbo;
		GLuint rgbTexture;
		GLuint depthTexture;

	public:
		Framebuffer(int width, int height) {
			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

			glGenTextures(1, &rgbTexture);
			glBindTexture(GL_TEXTURE_2D, rgbTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);

			glGenTextures(1, &depthTexture);
			glBindTexture(GL_TEXTURE_2D, depthTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rgbTexture, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				logError("Framebuffer incomplete!");

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		~Framebuffer() {
			glDeleteTextures(1, &depthTexture);
			glDeleteTextures(1, &rgbTexture);
			glDeleteFramebuffers(1, &fbo);
		}

		GLuint getRGBTextureID() {
			return rgbTexture;
		}

		GLuint getDepthTextureID() {
			return depthTexture;
		}

		GLuint getFboID() {
			return fbo;
		}

		Framebuffer(const Framebuffer& other) = delete; // copy constructor
		Framebuffer(Framebuffer&& other) = default; // move constructor
		Framebuffer& operator=(const Framebuffer& other) = delete; // copy assignment
		Framebuffer& operator=(Framebuffer&& other) = delete; // move assignment
	};
}

