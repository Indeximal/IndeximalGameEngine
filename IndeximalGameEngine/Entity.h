// Created by Indeximal 4/21/2018 2:11:04 PM
#pragma once

#include "IndeximalGameEngine.h"

namespace ige {
	template <class ShaderClass>
	class Entity {
	private:
		const Model& model;
		const Texture& texture;
		ShaderClass& shader;
		glm::mat4 modelMatrix;

	public:
		Entity(const Model& model, const Texture& texture, ShaderClass& shader, glm::vec3 position = glm::vec3(), float scalar = 1.0f, float zRotation = 0.0f)
			: model(model), texture(texture), shader(shader)
		{
			scale(scalar);
			rotate(zRotation, upwardsVector);
			move(position);
		}

		void render() {
			shader.start();
			shader.texture = texture;
			shader.modelMatrix = modelMatrix;
			model._render();
		}

		void move(glm::vec3 offset) {
			modelMatrix = glm::translate(glm::mat4(), offset) * modelMatrix;
		}

		void rotate(float angleRadians, glm::vec3 axis) {
			modelMatrix = glm::rotate(glm::mat4(), angleRadians, axis) * modelMatrix;
		}

		void scale(glm::vec3 scalar) {
			modelMatrix = glm::scale(glm::mat4(), scalar) * modelMatrix;
		}

		void scale(float scalar) {
			scale(glm::vec3(scalar));
		}

		void resetPositionRotationScale() {
			modelMatrix = glm::mat4();
		}
	};

}