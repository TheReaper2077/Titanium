#pragma once

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ti {
	namespace Component {
		struct Transform {
			glm::vec3 position = glm::vec3(0, 0, 0);
			glm::vec3 rotation = glm::vec3(0, 0, 0);
			glm::vec3 scale = glm::vec3(1, 1, 1);

			glm::mat4 GetTranslationMatrix() {
				return glm::translate(glm::mat4(1.0f), position);
			}

			glm::mat4 GetScaleMatrix() {
				return glm::scale(glm::mat4(1.0f), scale);
			}

			glm::quat GetRotationQuat() {
				return glm::quat(glm::radians(rotation));
			}

			glm::mat4 GetRotationMatrix() {
				return glm::mat4_cast(glm::conjugate(GetRotationQuat()));
			}

			glm::mat4 GetModel() {
				return GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
			}

			void Clear() {
				position = glm::vec3(0, 0, 0);
				scale = glm::vec3(0, 0, 0);
				rotation = glm::vec3(0, 0, 0);
			}
		};
	}
}