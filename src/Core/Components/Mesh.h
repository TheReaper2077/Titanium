#pragma once

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_cross_product.hpp>

#include <OpenGL.h>
#include <vector>

namespace ti {
	struct Vertex {
		glm::vec3 pos;
		glm::vec3 attr;
		glm::vec3 normal;

		static VertexArray* GetVertexArray() {
			static VertexArray* vertexarray;

			if (vertexarray == nullptr)
				vertexarray = VertexArray_Create({{ 0, 3, GL_FLOAT }, { 1, 3, GL_FLOAT }, { 2, 3, GL_FLOAT }});

			return vertexarray;
		}
	};

	namespace Component {
		struct Mesh {
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			
			unsigned int vertexcount = 0;
			unsigned int indexcount = 0;
		};
	}
}