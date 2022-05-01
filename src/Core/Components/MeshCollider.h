#pragma once

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_cross_product.hpp>

#include "MeshFilter.h"

namespace ti {
	namespace Component {
		struct MeshCollider {
			Mesh* mesh;
			bool convex;
		};
	}
}