// Created by Indeximal 4/19/2018 11:57:44 AM
#pragma once

#include "IndeximalGameEngine.h"
#include <random>

namespace ige {
	float* newValueNoiseArray(unsigned long long seed, int xCount, int yCount, int octaves, float freq, float freqMod, float amplitude, float ampMod) {
		float *heights = new float[xCount * yCount];
		for (int i = 0; i < xCount; i++) {
			for (int j = 0; j < yCount; j++) {
				heights[i * yCount + j] = 0.0f;
			}
		}
		std::mt19937_64 engine(seed);
		std::uniform_real_distribution<float> dist;
		dist(engine); dist(engine); dist(engine);
		for (int o = 0; o < octaves; o++) {
			float frequency = std::powf(freqMod, (float)o) * freq;
			float oAmp = std::powf(ampMod, (float)o) * amplitude;
			int xpoints = (int)(xCount * frequency) + 2;
			int ypoints = (int)(yCount * frequency) + 2;
			float *octaveHeights = new float[xpoints * ypoints];
			for (int i = 0; i < xpoints; i++) {
				for (int j = 0; j < ypoints; j++) {
					octaveHeights[i * ypoints + j] = dist(engine) * oAmp * 2 - oAmp;
				}
			}
			for (int x = 0; x < xCount; x++) {
				for (int y = 0; y < yCount; y++) {
					float xx = x * frequency;
					float yy = y * frequency;
					int x0 = (int)xx; int x1 = x0 + 1;
					int y0 = (int)yy; int y1 = y0 + 1;
					float v00 = octaveHeights[x0 * ypoints + y0];
					float v01 = octaveHeights[x1 * ypoints + y0];
					float v10 = octaveHeights[x0 * ypoints + y1];
					float v11 = octaveHeights[x1 * ypoints + y1];
					float wx = xx - x0;
					float v0 = v00 + wx * (v01 - v00);
					float v1 = v10 + wx * (v11 - v10);
					float wy = yy - y0;
					float v = v0 + wy * (v1 - v0);

					heights[x * yCount + y] += v;
				}
			}
			delete[] octaveHeights;
		}

		return heights;
	}

	class TerrainShader : public ShaderProgram {
	public:
		UniformMat4 modelMatrix;
		UniformMat4 viewMatrix;
		UniformMat4 projectionMatrix;
		UniformVec3 lightDirection;
		UniformVec3 color;
		UniformVec3 viewPosition;

		TerrainShader()
			: ShaderProgram("TerrainShader.vert", "TerrainShader.frag", { "position", "normal", "uv_coord" }),
			modelMatrix("modelMat", shaderProgram),
			viewMatrix("viewMat", shaderProgram),
			projectionMatrix("projMat", shaderProgram),
			lightDirection("lightDirection", shaderProgram),
			viewPosition("viewPosition", shaderProgram),
			color("color", shaderProgram)
		{}
	};


	class Terrain {
	private:
		Model * terrainModel;
		TerrainShader shader;
		int m_xCount;
		int m_yCount;
		float m_tileSize;

		glm::mat4 modelMat;

	public:
		Terrain(unsigned long long seed, int xCount, int yCount, float tileSize, int octaves, float freq, float freqMod, float amplitude, float ampMod)
			: m_xCount(xCount), m_yCount(yCount), m_tileSize(tileSize)
		{
			float *heights = newValueNoiseArray(seed, xCount + 1, yCount + 1, octaves, freq, freqMod, amplitude, ampMod);
			int arrWidth = yCount + 1;
			std::vector<Vertex> vertices;
			std::vector<GLuint> indices;
			for (int x = 0; x < xCount + 1; x++) {
				for (int y = 0; y < yCount + 1; y++) {
					if (x == xCount || y == yCount) {
						float h = heights[x * arrWidth + y];
						Vertex v0 = { (float)x, (float)y, h, 0.0f, 0.0f, 0.0f, (float)(x % 2), (float)(y % 2) };
						Vertex v1 = { (float)x, (float)y, h, 0.0f, 0.0f, 0.0f, (float)(x % 2), (float)(y % 2) };
						vertices.push_back(v0);
						vertices.push_back(v1);
						continue;
					}

					float h00 = heights[x * arrWidth + y];
					float h01 = heights[x * arrWidth + y + 1];
					float h10 = heights[(x + 1) * arrWidth + y];
					float h11 = heights[(x + 1) * arrWidth + y + 1];
					glm::vec3 vec0(x, y, h00);		// 0   0-----3 // (x *yC + y) * 2
					glm::vec3 vec1(x, y+1, h01);	// 1   | \   | //
					glm::vec3 vec3(x+1, y, h10);	// 3   |   \ |
					glm::vec3 vec2(x+1, y+1, h11);	// 2   1-----2
					glm::vec3 n0 = glm::cross(vec2 - vec0, vec1 - vec0);
					glm::vec3 n1 = glm::cross(vec3 - vec0, vec2 - vec0);
					Vertex v0 = { (float)x, (float)y, h00, n0.x, n0.y, n0.z, (float)(x % 2), (float)(y % 2) };
					Vertex v1 = { (float)x, (float)y, h00, n1.x, n1.y, n1.z, (float)(x % 2), (float)(y % 2) };
					vertices.push_back(v0);
					vertices.push_back(v1);
					indices.push_back((x	* arrWidth + y   ) * 2);
					indices.push_back(((x+1)* arrWidth + y+1 ) * 2);
					indices.push_back((x	* arrWidth + y+1 ) * 2);
					indices.push_back((x	* arrWidth + y   ) * 2 + 1);
					indices.push_back(((x+1)* arrWidth + y   ) * 2 + 1);
					indices.push_back(((x+1)* arrWidth + y+1 ) * 2 + 1);
				}
			}
			terrainModel = new Model(vertices, indices);
			delete[] heights;
			logInfo("Terrain generated (" + std::to_string(terrainModel->getMemorySize() / 1000) + "KB)");

			shader.color = { 0.2f, 0.8f, 0.1f };
			modelMat = glm::scale(glm::mat4(), glm::vec3(tileSize, tileSize, 1.0f));
		}

		void render(const glm::mat4& viewMat, const glm::vec3& viewPos, const glm::mat4& projMat, const glm::vec3& lightDir) {
			glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
			shader.start();
			shader.modelMatrix = modelMat;
			shader.viewMatrix = viewMat;
			shader.projectionMatrix = projMat;
			shader.viewPosition = viewPos;
			shader.lightDirection = lightDir;
			terrainModel->_render();
			shader.stop();
		}

		void render(const Camera& camera, const glm::vec3& lightDir) {
			render(camera.getViewMatrix(), camera.getPosition(), camera.getProjectionMatrix(), lightDir);
		}

		void move(const glm::vec3& offset) {
			modelMat = glm::translate(glm::mat4(), offset) * modelMat;
		}

		void destroy() {
			terrainModel->~Model();
			delete terrainModel;
		}

		~Terrain() {
			destroy();
		}

		Terrain(const Terrain& other) = delete; // copy constructor
		Terrain(Terrain&& other) = delete; // move constructor
		Terrain& operator=(const Terrain& other) = delete; // copy assignment
		Terrain& operator=(Terrain&& other) = delete; // move assignment
	};
}