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
	namespace Component {
		struct MeshRenderer {
			std::string material;

			VertexArray* vertexarray;
			VertexBuffer* vertexbuffer;
			IndexBuffer* indexbuffer;

			int indexcount;
			int vertexcount;
		};
	}
}