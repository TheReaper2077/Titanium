#pragma once

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_cross_product.hpp>

namespace ti {
	namespace Component {
		enum Projection {
			ORTHOGRAPIC,
			PERSPECTIVE
		};

		struct Camera {
			glm::mat4 projection;
			glm::mat4 view;
			glm::mat4 model;

			glm::vec3 Up;
			glm::vec3 Right;
			glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		};
	}
}