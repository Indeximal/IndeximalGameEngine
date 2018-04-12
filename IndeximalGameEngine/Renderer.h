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
				glViewport(0, 0, disp->getWidth(), disp->getHeight());
				INTERNAL::activeDisplay = disp;
			}
		}
		else {
			INTERNAL::activeDisplay = nullptr;
		}
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

}