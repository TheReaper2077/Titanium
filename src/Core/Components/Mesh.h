#pragma once

#include <glm/vec3.hpp>

#include <OpenGL.h>
#include <vector>

namespace ti {
	struct VertexN {
		glm::vec3 pos;
		glm::vec2 uv;
		glm::vec3 normal;

		static VertexArray* GetVertexArray() {
			static VertexArray* vertexarray;

			if (vertexarray == nullptr)
				vertexarray = VertexArray_Create({{ 0, 3, GL_FLOAT }, { 1, 2, GL_FLOAT }, { 2, 3, GL_FLOAT }});

			return vertexarray;
		}
	};

	namespace Component {
		struct Mesh {
			std::vector<VertexN> vertices;
			std::vector<unsigned int> indices;
			
			unsigned int vertexcount = 0;
			unsigned int indexcount = 0;
		};
	}
}