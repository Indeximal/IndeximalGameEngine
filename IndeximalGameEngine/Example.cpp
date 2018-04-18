// Created by Indeximal 08.04.2018

#include "IndeximalGameEngine.h"

bool wireframeOn = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		wireframeOn = !wireframeOn;
	}
}

int main(int argc, char *argv[]) {

	ige::Display display(800, 600, false, "Example Display");
	display.setBackgroundColor(0.7f, 0.7f, 0.7f);
	glfwSetKeyCallback(display.getNativeWindowPtr(), key_callback);

	ige::Framebuffer framebuffer(display.getWidth(), display.getHeight());

	glm::vec3 viewPos = glm::vec3(1.0f, 2.0f, 2.0f);
	glm::mat4 viewMat = glm::lookAt(viewPos, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 projMat = glm::perspective(glm::radians(80.0f), display.getAspectRatio(), 0.1f, 10.0f);


	//ige::Model frog = ige::loadModelFromObjFile("res/Frog.obj");
	ige::Model dog = ige::loadModelFromObjFile("res/LowPolyDog.obj");
	glm::mat4 modelMat = glm::scale(glm::mat4(), glm::vec3(1.0f));

	//ige::renderToDisplay(&display);
	//glViewport(0, 0, display.getWidth() / 2, display.getHeight() / 2);
	//ige::setCulling(true);

	ige::StaticShader shader;

	shader.color = glm::vec3(153.0f, 107.0f, 38.0f) / 256.0f;
	shader.lightDirection = { 0.0f, -1.0f, -1.0f };
	shader.viewPosition = viewPos;
	shader.viewMatrix = viewMat;
	shader.projectionMatrix = projMat;

	ige::Quad topLeftQuad(-1.0f, 0.0f, 0.0f, 1.0f);
	ige::Quad topRightQuad(0.0f, 0.0f, 1.0f, 1.0f);

	ige::Shader2D quadShader;

	while (display.update()) {
		glm::mat4 rotatedModel = glm::rotate(modelMat, (float) ige::getTimeSinceStart() * 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
		shader.modelMatrix = rotatedModel;
		
		ige::setWireframe(false);
		ige::renderToFramebuffer(framebuffer);
		ige::useShader(&shader);
		ige::renderModel(dog);
		//ige::renderModel(frog);

		ige::setWireframe(wireframeOn);
		ige::renderToDisplay(&display);
		ige::useShader(&quadShader);
		quadShader.setTexture(framebuffer.getDepthTextureID());
		ige::renderQuad(topRightQuad);
		quadShader.setTexture(framebuffer.getRGBTextureID());
		ige::renderQuad(topLeftQuad);

	}

	return 0;
}