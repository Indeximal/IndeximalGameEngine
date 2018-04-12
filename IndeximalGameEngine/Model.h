// Created by Indeximal 09.04.2018

#pragma once

#include "IndeximalGameEngine.h"

namespace ige {
	struct Vertex {
		GLfloat posX;
		GLfloat posY;
		GLfloat posZ;
		GLfloat normalX;
		GLfloat normalY;
		GLfloat normalZ;
	};

	class Model {
	private:
		GLuint vao;
		GLuint vertexVbo;
		GLuint normalVbo;
		GLuint elementBuffer;

		int dimension;
		int totalVertices;

	public:
		Model(const std::vector<GLfloat> vertices, const std::vector<GLfloat> normals, const std::vector<GLuint> indices)
			: dimension(3), totalVertices((int)indices.size())
		{
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			// vertex positions
			glGenBuffers(1, &vertexVbo);
			glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, dimension, GL_FLOAT, GL_FALSE, dimension * sizeof(GLfloat), 0);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// vertex positions
			glGenBuffers(1, &normalVbo);
			glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, dimension, GL_FLOAT, GL_FALSE, dimension * sizeof(GLfloat), 0);
			glEnableVertexAttribArray(1);
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
			glDeleteBuffers(1, &normalVbo);
			glDeleteBuffers(1, &elementBuffer);
			glDeleteVertexArrays(1, &vao);
		}
		
		Model(const Model& other) = delete; // copy constructor
		Model(Model&& other) = default; // move constructor
		Model& operator=(const Model& other) = delete; // copy assignment
		Model& operator=(Model&& other) = delete; // move assignment
	};

	Model loadModelFromObjFile(std::string objFile) {
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::string err;
		bool success = tinyobj::LoadObj(&attrib, &shapes, nullptr, &err, objFile.c_str());
		if (!success) {
			logError(err);
			throw std::logic_error("Couldnt load Obj File");
		}

		std::vector<GLfloat> vertices = attrib.vertices;
		std::vector<GLfloat> normals = attrib.normals;
		if (normals.size() != vertices.size())
			throw std::logic_error("Normals and Vertices don't match!");
		std::vector<GLuint> indices;

		auto meshIndices = shapes[0].mesh.indices;
		indices.resize(meshIndices.size());
		for (int i = 0; i < meshIndices.size(); i++) {
			indices[i] = meshIndices[i].vertex_index;
			auto nIndex = meshIndices[i].normal_index;
			if (indices[i] != nIndex)
				throw std::logic_error("Contradictory indices");
		}
		//logInfo(shapes.size() + " shape loaded");

		//for (auto shape : shapes) {
		//	size_t offset = 0;
		//	for (auto numVerticesInThisFace : shape.mesh.num_face_vertices) {
		//		if (numVerticesInThisFace != 3)
		//			logError("This obj contains non-triangle faces");
		//		for (int v = 0; v < numVerticesInThisFace; v++) {
		//			tinyobj::index_t index = shape.mesh.indices[offset + v];
		//			indices.push_back(index.vertex_index);
		//		}
		//		offset += numVerticesInThisFace;
		//	}
		//}

		return Model(vertices, normals, indices);
	}

}