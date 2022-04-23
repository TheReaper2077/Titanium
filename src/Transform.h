#pragma once

#include "define.h"

namespace ti {
	struct Transform {
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;

		glm::mat4 GetTranslationMatrix() {
			return glm::translate(glm::mat4(1.0f), position);
		}

		glm::mat4 GetRotationMatrix() {
			auto model = glm::mat4(1.0f);
			model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
			model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
			model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
			return model;
		}
	};
}