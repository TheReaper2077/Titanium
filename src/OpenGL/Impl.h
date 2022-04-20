#pragma once

#ifndef GL_API
#include "OpenGL.h"
#endif

struct OpenGLContext {
	std::vector<std::shared_ptr<VertexArray>> vertex_array_store;
	std::vector<std::shared_ptr<IndexBuffer>> index_buffer_store;
	std::vector<std::shared_ptr<VertexBuffer>> vertex_buffer_store;
	std::vector<std::shared_ptr<UniformBuffer>> uniform_buffer_store;
	std::vector<std::shared_ptr<Shader>> shader_store;
	std::vector<std::shared_ptr<Texture>> texture_store;
	std::vector<std::shared_ptr<TextureArray>> sprite_atlas_store;

	VertexArrayId binding_vertexarray;
	VertexBufferId binding_vertexbuffer;
	IndexBufferId binding_indexbuffer;
	UniformBufferId binding_uniformbuffer;
	ShaderId binding_shader;

	int MAX_TEXTURES = 0;

	OpenGLContext() {}
};


struct VertexArrayLayout {
	unsigned int idx, size, type;

	VertexArrayLayout(unsigned int idx, unsigned int size, unsigned int type) {
		this->idx = idx;
		this->size = size;
		this->type = type;
	}
};

struct VertexArray {
	VertexArrayId id;
	uint32_t stride = 0;
};

enum BufferType {
	STATIC,
	DYNAMIC
};

struct VertexBuffer {
	VertexBufferId id;
	BufferType type;
	std::size_t size = 0;
};


struct UniformBuffer {
	UniformBufferId id;
	std::size_t size = 0;
};

struct IndexBuffer {
	IndexBufferId id;
	std::size_t size = 0;
	VertexArray *vertexarray;
};

struct Shader {
	ShaderId id;
	std::unordered_map<std::string, unsigned int> uniform_location_map;

	bool textures = false;
	bool texture_array = false;
	bool material = false;
	bool light = false;
};

struct Texture {
	TextureId id;
	uint32_t width, height, channels;
};

struct TextureArray {
	Texture *texture = nullptr;

	int tilecount;
	int tileheight;
	int tilewidth;
	int columns;
};
