#include "Framebuffer.h"

namespace ige {

	Framebuffer::Framebuffer(int width, int height) {
		
	}

	Framebuffer::~Framebuffer() {
		glDeleteTextures(1, &depthTexture);
		glDeleteTextures(1, &rgbTexture);
		glDeleteFramebuffers(1, &fbo);
	}
}

