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
		struct Tilemap {
			int* tiles;
			int xmax;
			int ymax;
			int xgrid;
			int ygrid;
		};
	}
}