#pragma once

#include <glm/vec3.hpp>

#include <OpenGL.h>
#include <vector>
#include "Material.h"
#include "Mesh.h"
#include "../Enumerations.h"

namespace ti {
	namespace Component {
		struct MeshFilter {
			std::string mesh;
			bool init = false;

			MeshFilter() {}
		};
	}
}