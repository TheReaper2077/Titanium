// #pragma once

// #include <glm/glm.hpp>
// #include <glm/vec2.hpp>
// #include <glm/vec3.hpp>
// #include <glm/vec4.hpp>
// #include <glm/geometric.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtx/matrix_cross_product.hpp>

// namespace ti {
// 	struct Transform {
// 		glm::vec3 position;
// 		glm::vec3 scale;
// 		glm::vec3 rotation;

// 		glm::mat4 GetTranslationMatrix() {
// 			return glm::translate(glm::mat4(1.0f), position);
// 		}

// 		glm::mat4 GetScaleMatrix() {
// 			return glm::scale(glm::mat4(1.0f), scale);
// 		}

// 		glm::mat4 GetRotationMatrix() {
// 			auto model = glm::mat4(1.0f);
// 			model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
// 			model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
// 			model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
// 			return model;
// 		}

// 		glm::mat4 GetModel() {
// 			return GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
// 		}

// 		void Clear() {
// 			position = glm::vec3(0, 0, 0);
// 			scale = glm::vec3(0, 0, 0);
// 			rotation = glm::vec3(0, 0, 0);
// 		}
// 	};
// }