#pragma once

#include <glm/vec3.hpp>

#include <OpenGL.h>
#include <vector>
#include "Material.h"

namespace ti {

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

	struct Vertex {
		glm::vec3 position;
		glm::vec3 uv;
		glm::vec3 normal;
	};

	enum Primitive {
		POINT,
		LINE,
		LINE_STRIP,
		TRIANGLE,
		TRIANGLE_FAN,
		TRIANGLE_STRIP,
	};

	namespace Component {
		struct Mesh {
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			
			unsigned int vertexcount = 0;
			unsigned int indexcount = 0;

			VertexBuffer* vertexbuffer = nullptr;
			VertexArray* vertexarray = nullptr;
			IndexBuffer* indexbuffer = nullptr;

			std::string material;
			Primitive primitive = TRIANGLE;

			bool changed = true;

			void Clear();
		};
	}
}