// Created by Indeximal 09.04.2018

#pragma once

#include "IndeximalGameEngine.h"
#include <unordered_map>

namespace tinyobj {
	bool operator==(const index_t& a, const index_t& b) {
		return a.normal_index == b.normal_index 
			&& a.vertex_index == b.vertex_index 
			&& a.texcoord_index == b.texcoord_index;
	}
}

namespace std {
	template<>
	struct hash<tinyobj::index_t> {
		std::size_t operator()(const tinyobj::index_t& a) const {
			size_t hash = 17;
			hash = hash * 31 + std::hash<int>()(a.vertex_index);
			hash = hash * 31 + std::hash<int>()(a.normal_index);
			hash = hash * 31 + std::hash<int>()(a.texcoord_index);
			return hash;
		}
	};
}

namespace ige {
	struct Vertex {
		GLfloat posX;
		GLfloat posY;
		GLfloat posZ;
		GLfloat normalX;
		GLfloat normalY;
		GLfloat normalZ;
		GLfloat texU;
		GLfloat texV;
	};

	struct Vertex2D {
		GLfloat posX;
		GLfloat posY;
		GLfloat texU;
		GLfloat texV;
	};

	class Quad {
	private:
		GLuint vao;
		GLuint vbo;

		Quad(std::vector<Vertex2D> vertices) {
			if (vertices.size() != 6)
				logError("This quad doesn't have 6 vertices!");


			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2D), vertices.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, posX));
			glEnableVertexAttribArray(0); // Position
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, texU));
			glEnableVertexAttribArray(1); // Tex Coords
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(0);
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
		}

	public:
		Quad(float posXMin, float posYMin, float posXMax, float posYMax, float texUMin = 0.0f, float texVMin = 0.0f, float texUMax = 1.0f, float texVMax = 1.0f) 
			: Quad({
				{ posXMin, posYMax, texUMin, texVMax },	// 0
				{ posXMin, posYMin, texUMin, texVMin },	// 1   // 0-----3
				{ posXMax, posYMin, texUMax, texVMin },	// 2   // | \   |
				{ posXMin, posYMax, texUMin, texVMax },	// 0   // |   \ |
				{ posXMax, posYMin, texUMax, texVMin },	// 2   // 1-----2
				{ posXMax, posYMax, texUMax, texVMax },	// 3														
			})
		{}

		Quad(): Quad(-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f) {}

		void _render() {
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}

		~Quad() {
			glDeleteBuffers(1, &vbo);
			glDeleteVertexArrays(1, &vao);
		}

		Quad(const Quad& other) = delete; // copy constructor
		Quad(Quad&& other) = default; // move constructor
		Quad& operator=(const Quad& other) = delete; // copy assignment
		Quad& operator=(Quad&& other) = delete; // move assignment
	};

	class Model {
	private:
		GLuint vao;
		GLuint vertexVbo;
		GLuint elementBuffer;

		int dimension;
		int totalVertices;

		size_t memorySize;

		void load(const std::vector<Vertex> vertices, const std::vector<GLuint> indices) {
			totalVertices = (int)indices.size();
			memorySize = vertices.size() * sizeof(Vertex) + indices.size() * sizeof(GLuint);

			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			// vertices
			glGenBuffers(1, &vertexVbo);
			glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, posX));
			glEnableVertexAttribArray(0); // Position
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normalX));
			glEnableVertexAttribArray(1); // Normal
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texU));
			glEnableVertexAttribArray(2); // UV Coordinates
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// indices
			glGenBuffers(1, &elementBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

			glBindVertexArray(0);
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
		}

	public:
		Model(const std::vector<Vertex> vertices, const std::vector<GLuint> indices) {
			load(vertices, indices);
		}

		Model(const std::string objFile) {
			tinyobj::attrib_t dataArrays;
			std::vector<tinyobj::shape_t> shapes;
			std::string err;
			bool success = tinyobj::LoadObj(&dataArrays, &shapes, nullptr, &err, objFile.c_str());
			if (!success)
				throw std::invalid_argument("Couldnt load Obj File" + err);

			if (shapes.size() != 1)
				logWarning("The obj file '" + objFile + "' contains more than one shape, but only one will be loaded.");

			std::vector<Vertex> vertices;
			std::vector<GLuint> indices;

			auto meshIndices = shapes[0].mesh.indices;
			indices.resize(meshIndices.size());

			std::unordered_map<tinyobj::index_t, GLuint> indexMap;
			indexMap.reserve((size_t)(meshIndices.size() * 0.5));

			if (dataArrays.vertices.size() == 0) throw std::logic_error("Vertex Positions not availiable.");
			if (dataArrays.normals.size() == 0) logWarning(objFile + ": Model Normals aren't availiable.");
			if (dataArrays.texcoords.size() == 0) logWarning(objFile + ": Model Texcoords aren't availiable.");

			for (int i = 0; i < meshIndices.size(); i++) {
				auto indexT = meshIndices[i];
				Vertex vertex;
				vertex.posX = dataArrays.vertices[indexT.vertex_index * 3];
				vertex.posY = dataArrays.vertices[indexT.vertex_index * 3 + 1];
				vertex.posZ = dataArrays.vertices[indexT.vertex_index * 3 + 2];
				if (indexT.normal_index != -1) {
					vertex.normalX = dataArrays.normals[indexT.normal_index * 3];
					vertex.normalY = dataArrays.normals[indexT.normal_index * 3 + 1];
					vertex.normalZ = dataArrays.normals[indexT.normal_index * 3 + 2];
				}
				else {
					vertex.normalX = 0.0f; vertex.normalY = 0.0f; vertex.normalZ = 0.0f;
				}
				if (indexT.texcoord_index != -1) {
					vertex.texU = dataArrays.texcoords[indexT.texcoord_index * 2];
					vertex.texV = dataArrays.texcoords[indexT.texcoord_index * 2 + 1];
				}
				else {
					vertex.texU = 0.0f; vertex.texV = 0.0f;
				}

				auto indexOrEnd = indexMap.find(indexT);
				if (indexOrEnd == indexMap.end()) {
					indexMap[indexT] = (GLuint)vertices.size(); // Add index to hashtable
					indices[i] = (GLuint)vertices.size(); // add index to element buffer
					vertices.push_back(vertex); // add vertex to vertex buffer
				}
				else {
					indices[i] = indexOrEnd->second;
				}
			}
			load(vertices, indices);

			logInfo(objFile + ": " + std::to_string(indices.size() / 3) + " triangles with " +
				std::to_string(vertices.size()) + " vertices successfully loaded. (" +
				std::to_string(memorySize / 1000) + "KB)");
		}

		void _render() const {
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, totalVertices, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}

		int getMemorySize() const {
			return memorySize;
		}

		~Model() {
			glDeleteBuffers(1, &vertexVbo);
			glDeleteBuffers(1, &elementBuffer);
			glDeleteVertexArrays(1, &vao);
		}
		
		Model(const Model& other) = delete; // copy constructor
		Model(Model&& other) = default; // move constructor
		Model& operator=(const Model& other) = delete; // copy assignment
		Model& operator=(Model&& other) = delete; // move assignment
	};
}