#pragma once

#include "define.h"
#include <OpenGL.h>
#include "Vertex.h"
#include "Material.h"

namespace ti {
	enum Primitive {
		POINT,
		LINE,
		TRIANGLE,
		QUAD,
	};

	struct Mesh {
		VertexArray* vertexarray;
		VertexBuffer* vertexbuffer;
		IndexBuffer* indexbuffer;

		Primitive primitive;
		bool indexed = false;

		uint32_t vertexcount;
		std::vector<Vertex> vertices;
		uint32_t indexcount;
		std::vector<uint32_t> indices;

		// std::unordered_map<Texture*, uint32_t> texture_map;

		Material material;

		Mesh(Primitive primitive, Material material);

		void RecalculateNormals();

		void Transfer();

		void Clear();
	};
}