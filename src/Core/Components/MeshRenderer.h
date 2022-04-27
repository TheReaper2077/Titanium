#pragma once

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_cross_product.hpp>
#include <OpenGL.h>

namespace ti {
	// struct Material {
	// 	glm::vec3 ambient;
	// 	glm::vec3 diffuse;
	// 	glm::vec3 specular;
	// 	float shininess;

	// 	Texture* diffuse_map;
	// 	Texture* specular_map;
	// 	Texture* ambient_map;
	// };

	namespace Component {
		struct MeshRenderer {
			// Material material;
			bool cleanup = true;
		};
	}
}