#pragma once

#include "../Vertex.h"

namespace ti {
	namespace Component {
		struct Mesh {
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			
			unsigned int vertexcount = 0;
			unsigned int indexcount = 0;
		};
	}
}