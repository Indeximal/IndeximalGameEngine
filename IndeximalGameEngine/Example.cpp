// Created by Indeximal 08.04.2018

#include "IndeximalGameEngine.h"


int main(int argc, char *argv[]) {

	ige::Display display(800, 600, false, "Example Display");
	display.setBackgroundColor(0.7f, 0.7f, 0.7f);

	glm::vec3 viewPos = glm::vec3(1.0f, 2.0f, 2.0f);

	glm::mat4 viewMat = glm::lookAt(viewPos, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 projMat = glm::perspective(glm::radians(80.0f), display.getAspectRatio(), 0.1f, 10.0f);

	ige::StaticShader shader;

	std::vector<GLfloat> verts = { 
		-.5f, .0f, 0.5f,	// 0-----3
		-.5f, .0f, -.5f,	// | \   |
		0.5f, .0f, -.5f,	// |   \ |
		0.5f, .0f, 0.5f		// 1-----2
	};
	std::vector<GLuint> indices = { 0, 1, 2, 0, 2, 3 };

	//ige::Model quad(verts, indices);
	ige::Model dog = ige::loadModelFromObjFile("res/LowPolyDog.obj");
	glm::mat4 modelMat = glm::scale(glm::mat4(), glm::vec3(1.0f));

	ige::renderToDisplay(&display);
	//ige::setWireframe(true);
	ige::setCulling(false);

	shader.color = glm::vec3(153.0f, 107.0f, 38.0f) / 256.0f;
	shader.lightDirection = { 0.0f, -1.0f, -1.0f };
	shader.viewPosition = viewPos;

	while (display.update()) {
		glm::mat4 rotatedModel = glm::rotate(modelMat, (float) ige::getTimeSinceStart() * 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
		ige::useShader(&shader);
		shader.modelMatrix = rotatedModel;
		shader.viewMatrix = viewMat;
		shader.projectionMatrix = projMat;
		ige::renderModel(dog);

		//shader.setMVP(projMat * viewMat * glm::mat4());
		//ige::renderModel(quad);

	}

	return 0;
}