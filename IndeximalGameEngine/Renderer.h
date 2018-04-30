// Created by Indeximal 10.04.2018
#pragma once

#include "IndeximalGameEngine.h"

namespace ige {
	namespace INTERNAL {
		Display *activeDisplay = nullptr;
	}

	void renderToDisplay(Display *disp) {
		if (disp != nullptr) {
			if (disp != INTERNAL::activeDisplay) {
				disp->_makeContext();
				//logInfo("OpenGL (" + getOpenGLVersion() + ") has been started.");
				INTERNAL::activeDisplay = disp;
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, disp->getWidth(), disp->getHeight());

		} else {
			INTERNAL::activeDisplay = nullptr;
		}
	}

	void renderToFramebuffer(Framebuffer &framebuffer) {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.getNativeFboID());
		glViewport(0, 0, framebuffer.getWidth(), framebuffer.getHeight());
		std::vector<GLuint>& attachments = framebuffer.getNativeAttachments();
		glDrawBuffers((GLsizei) attachments.size(), attachments.data());
	}

	void setWireframe(bool state) {
		if (state) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	void setCulling(bool enable) {
		if (enable) {
			glEnable(GL_CULL_FACE);
		} else {
			glDisable(GL_CULL_FACE);
		}
	}

	void setDepthTest(bool depthTestEnabled) {
		if (depthTestEnabled) {
			glEnable(GL_DEPTH_TEST);
		}
		else {
			glDisable(GL_DEPTH_TEST);
		}
	}

	void useShader(ShaderProgram *shader) {
		if (shader != nullptr) {
			shader->start();
		}
		else {
			glUseProgram(0);
		}
	}

	void renderModel(Model &model) {
		model._render();
	}

	void renderQuad(Quad &quad) {

		quad._render();
	}

	void setAlphaBlending(bool blendOn) {
		if (blendOn) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		} else {
			glDisable(GL_BLEND);
		}
	}

}