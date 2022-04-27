#pragma once

#include <glm/vec3.hpp>

#include <OpenGL.h>
#include <vector>

namespace ti {
	// enum VertexArrayAttrib {
	// 	position,
	// 	normal,
	// 	color,
	// 	uv0,
	// 	uv1,
	// 	uv2,
	// 	uv3,
	// 	uv4,
	// 	uv5,
	// 	uv6,
	// 	uv7,
	// };

	// using VertexArrayDescriptor = uint32_t;

	// #define POSITION_ATTR_BIT (1 << ti::VertexArrayAttrib::position)
	// #define NORMAL_ATTR_BIT (1 << ti::VertexArrayAttrib::normal)
	// #define COLOR_ATTR_BIT (1 << ti::VertexArrayAttrib::color)
	// #define UV0_ATTR_BIT (1 << ti::VertexArrayAttrib::uv0)
	// #define UV1_ATTR_BIT (1 << ti::VertexArrayAttrib::uv1)
	// #define UV2_ATTR_BIT (1 << ti::VertexArrayAttrib::uv2)
	// #define UV3_ATTR_BIT (1 << ti::VertexArrayAttrib::uv3)
	// #define UV4_ATTR_BIT (1 << ti::VertexArrayAttrib::uv4)
	// #define UV5_ATTR_BIT (1 << ti::VertexArrayAttrib::uv5)
	// #define UV6_ATTR_BIT (1 << ti::VertexArrayAttrib::uv6)
	// #define UV7_ATTR_BIT (1 << ti::VertexArrayAttrib::uv7)

	// #define POSITION_ATTR_SIZE sizeof(glm::vec3)
	// #define NORMAL_ATTR_SIZE sizeof(glm::vec3)
	// #define COLOR_ATTR_SIZE sizeof(glm::vec3)
	// #define UV0_ATTR_SIZE sizeof(glm::vec2)
	// #define UV1_ATTR_SIZE sizeof(glm::vec2)
	// #define UV2_ATTR_SIZE sizeof(glm::vec2)
	// #define UV3_ATTR_SIZE sizeof(glm::vec2)
	// #define UV4_ATTR_SIZE sizeof(glm::vec2)
	// #define UV5_ATTR_SIZE sizeof(glm::vec2)
	// #define UV6_ATTR_SIZE sizeof(glm::vec2)
	// #define UV7_ATTR_SIZE sizeof(glm::vec2)

	namespace Component {
		struct Mesh {
			void* vertices = nullptr;
			std::size_t size = 0;
			std::vector<unsigned int> indices;
			
			unsigned int vertexcount = 0;
			unsigned int indexcount = 0;

			VertexArrayLayout vertexarraydesc;

			void Clear();
			void AppendVertices();
		};
	}
}