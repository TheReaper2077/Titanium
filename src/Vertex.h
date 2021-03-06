#pragma once

#include <OpenGL.h>
#include "define.h"
#include "math/math.h"

namespace ti {
	struct Vertex {
		Vec3 pos;
		Vec3 attr;
		Vec3 normal;

		static VertexArray* GetVertexArray() {
			static VertexArray* vertexarray;

			if (vertexarray == nullptr)
				vertexarray = VertexArray_Create({{ 0, 3, GL_FLOAT }, { 1, 3, GL_FLOAT }, { 2, 3, GL_FLOAT }});

			return vertexarray;
		}
	};
}