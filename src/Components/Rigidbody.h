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
		struct Rigidbody {
			glm::vec3 velocity;
			glm::vec3 acceleration;
			glm::vec3 force;
			float mass;
			bool use_gravity = true;

			float drag;
		};
	}
}