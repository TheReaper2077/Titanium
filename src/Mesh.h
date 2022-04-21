#pragma once

#include "define.h"
#include "OpenGL/OpenGL.h"

struct Mesh {
	VertexArray* vertexarray;
	VertexBuffer* vertexbuffer;
	IndexBuffer* indexbuffer;

	std::unordered_map<Texture*, uint32_t> texture_map;

	void RecalculateNormals();
	void Render();
};