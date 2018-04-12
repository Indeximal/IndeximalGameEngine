// Created by Indeximal 09.04.2018

#pragma once

#include "IndeximalGameEngine.h"

namespace ige {

	class Model {
	private:
		GLuint vao;
		GLuint vertexVbo;
		GLuint elementBuffer;

		int dimension;
		int totalVertices;

	public:
		Model(const std::vector<GLfloat> vertices, const std::vector<GLuint> indices)
			: dimension(3), totalVertices((int)indices.size())
		{
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			// vertex positions
			glGenBuffers(1, &vertexVbo);
			glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * dimension * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, dimension, GL_FLOAT, GL_FALSE, dimension * sizeof(GLfloat), 0);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// indices
			glGenBuffers(1, &elementBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

			glBindVertexArray(0);
		}

		void _render() {
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, totalVertices, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}

		~Model() {
			glDeleteBuffers(1, &vertexVbo);
			glDeleteBuffers(1, &elementBuffer);
			glDeleteVertexArrays(1, &vao);
		}
		
		Model(const Model& other) = delete; // copy constructor
		Model(Model&& other) = delete; // move constructor
		Model& operator=(const Model& other) = delete; // copy assignment
		Model& operator=(Model&& other) = delete; // move assignment
	};

	//Model loadModelFromObjFile() {
	//	return Model({}, {});
	//}

}