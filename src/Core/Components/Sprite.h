#pragma once

#include <OpenGL.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_cross_product.hpp>

namespace ti {
	namespace Component {
		struct Sprite {
			Texture *texture;
			glm::vec2 uv0;
			glm::vec2 uv1;
			float index;
		};
	}
}