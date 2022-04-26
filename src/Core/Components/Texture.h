#pragma once

#include <OpenGL.h>
#include <glm/vec3.hpp>

namespace ti {
	namespace Component {
		struct Texture {
			Texture *texture;
			glm::vec3 uv0;
			glm::vec3 uv1;
		};
	}
}