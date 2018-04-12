// Created by Indeximal 08.04.2018

#include "IndeximalGameEngine.h"


int main(int argc, char *argv[]) {

	ige::Display display(800, 600, false, "Example Display");
	display.setBackgroundColor(0.7f, 0.7f, 0.7f);

	glm::mat4 viewMat = glm::lookAt(glm::vec3(1.0f, 0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 projMat = glm::perspective(glm::radians(80.0f), display.getAspectRatio(), 0.1f, 10.0f);

	ige::StaticShader shader;
	shader.setTintColor(1, 0, 0);


	std::vector<GLfloat> verts = { 
		-.5f, 0.5f, .0f,	// 0-----3
		-.5f, -.5f, .0f,	// | \   |
		0.5f, -.5f, .0f,	// |   \ |
		0.5f, 0.5f, .0f		// 1-----2
	};
	std::vector<GLuint> indices = { 0, 1, 2, 0, 2, 3 };

	ige::Model quad(verts, indices);

	ige::renderToDisplay(&display);

	
	while (display.update()) {
		glm::mat4 modelMat = glm::rotate(glm::mat4(1.0f), (float) ige::getTimeSinceStart(), glm::vec3(0.0f, 0.0f, 1.0f));
		shader.setMVP(projMat * viewMat * modelMat);


		ige::useShader(&shader);
		ige::renderModel(quad);
	}

	return 0;
}