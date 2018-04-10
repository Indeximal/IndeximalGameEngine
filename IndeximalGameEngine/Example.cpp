// Created by Indeximal 08.04.2018

#include "IndeximalGameEngine.h"


int main(int argc, char *argv[]) {

	ige::Display display(800, 600, false, "Example Display");
	display.setBackgroundColor(0.7f, 0.7f, 0.7f);

	ige::StaticShader shader;

	std::vector<GLfloat> verts = { 
		-.5f, 0.5f, .0f,	// 0-----3
		-.5f, -.5f, .0f,	// | \   |
		0.5f, -.5f, .0f,	// |   \ |
		0.5f, 0.5f, .0f		// 1-----2
	};
	std::vector<GLuint> indices = { 0, 1, 2, 0, 2, 3 };

	ige::Model triangle(verts, indices);

	while (display.update()) {
		shader.start();
		//display.renderModel(triangle);
		triangle.render();
		shader.stop();

	}

	return 0;
}