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
		struct SpriteRenderer {
			glm::vec4 color = glm::vec4(1, 1, 1, 1);
			Texture2D* texture;

			bool lighting = false;
			bool visible = true;

			uint32_t flags = 0;
		};
	}
}