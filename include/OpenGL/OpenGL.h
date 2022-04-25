#pragma once

#define GL_API
#define GL_ASSERT(x) assert(x)

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <glad/glad.h>
#include <assert.h>
#include <stb_image.h>

typedef unsigned int VertexArrayId;
typedef unsigned int VertexBufferId;
typedef unsigned int IndexBufferId;
typedef unsigned int UniformBufferId;
typedef unsigned int ShaderId;
typedef unsigned int TextureId;
typedef unsigned int SpriteAtlasId;
typedef unsigned int FrameBufferId;
typedef unsigned int TextureColorBufferId;
typedef unsigned int RenderBufferId;

struct OpenGLContext;
struct VertexArrayLayout;
struct VertexArray;
struct VertexBuffer;
struct IndexBuffer;
struct UniformBuffer;
struct Shader;
struct Texture;
struct TextureArray;
struct FrameBuffer;
struct TextureColorBuffer;
struct RenderBuffer;

struct OpenGLContext {
	std::vector<std::shared_ptr<VertexArray>> vertex_array_store;
	std::vector<std::shared_ptr<IndexBuffer>> index_buffer_store;
	std::vector<std::shared_ptr<VertexBuffer>> vertex_buffer_store;
	std::vector<std::shared_ptr<UniformBuffer>> uniform_buffer_store;
	std::vector<std::shared_ptr<FrameBuffer>> frame_buffer_store;
	std::vector<std::shared_ptr<Shader>> shader_store;
	std::vector<std::shared_ptr<Texture>> texture_store;
	std::vector<std::shared_ptr<TextureArray>> sprite_atlas_store;

	VertexArrayId binding_vertexarray;
	VertexBufferId binding_vertexbuffer;
	IndexBufferId binding_indexbuffer;
	UniformBufferId binding_uniformbuffer;
	FrameBufferId binding_framebuffer;
	ShaderId binding_shader;

	int MAX_TEXTURES = 0;

	OpenGLContext() {}
};


struct FrameBuffer {
	FrameBufferId id;
	int width, height;

	void Bind();
	void UnBind();
	void Destroy();
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
	
	void Bind();
	void BindVertexBuffer(VertexBuffer* vertexbuffer, std::size_t stride = 0, std::size_t offset = 0);
	void BindIndexBuffer(IndexBuffer* indexbuffer);
	void UnBind();
	void Destroy();
};

enum BufferType {
	STATIC,
	DYNAMIC
};

struct VertexBuffer {
	VertexBufferId id;
	BufferType type;
	std::size_t size = 0;

	void Bind();
	void UnBind();
	void Allocate(std::size_t size);
	void AddDataDynamic(void* data, std::size_t size, std::size_t offset = 0);
	void AddDataStatic(void* data, std::size_t size);
	void Destroy();
};

struct IndexBuffer {
	IndexBufferId id;
	std::size_t size = 0;
	VertexArray* vertexarray;
	
	void Bind();
	void UnBind();
	void Allocate(std::size_t size);
	void AddData(unsigned int* data, std::size_t size, std::size_t offset = 0);
	void Destroy();
};

struct UniformBuffer {
	UniformBufferId id;
	std::size_t size = 0;
	
	void Allocate(std::size_t size);
	void AddDataDynamic(void* data, std::size_t size, std::size_t offset = 0);
	void BindRange(unsigned int index, std::size_t size, std::size_t offset = 0);
	void Bind();
	void UnBind();
};

struct Shader {
	ShaderId id;
	std::unordered_map<std::string, unsigned int> uniform_location_map;

	std::string name;

	bool textures = false;
	bool texture_array = false;
	bool material = false;
	bool light = false;
	
	void Bind();
	void UnBind();

	void BindUniformBlock(std::string name, unsigned int index);
	unsigned int GetUniformLoc(std::string name);

	void SetUniformi(std::string uniform, int v0);
	void SetUniformf(std::string uniform, float v0);
	void SetUniformVec3(std::string uniform, const float* v);
	void SetUniformVec4(std::string uniform, const float* v);
	void SetUniformMat4(std::string uniform, const float* matrix);
	void SetUniformArray(std::string uniform, std::size_t count, const float* v);
};

struct Texture {
	TextureId id;
	uint32_t width, height, channels;

	void Bind();
	void BindUnit(uint32_t unit);
	void UnBind();
};

struct TextureArray {
	Texture* texture = nullptr;

	int tilecount;
	int tileheight;
	int tilewidth;
	int columns;
	
	void Bind();
	void UnBind();
};

void OpenGL_CreateContext();
void OpenGL_DestroyContext();

FrameBuffer* FrameBuffer_Create(int width, int height);

VertexArray* VertexArray_Create(std::vector<VertexArrayLayout> layouts);

VertexBuffer* VertexBuffer_Create();

IndexBuffer* IndexBuffer_Create(VertexArray* vertexarray);

UniformBuffer* UniformBuffer_Create();

Shader* Shader_Create(std::string name, const std::string &vs_filename, const std::string &fs_filename, bool file = true);

Texture* Texture_Create();
Texture* LoadFile(const char* filename);
TextureArray* TextureArray_LoadFile(int tilew, int tileh, const char* filename);
