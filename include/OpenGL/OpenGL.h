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
struct VertexArrayAttribDescriptor;
struct VertexArray;
struct VertexBuffer;
struct IndexBuffer;
struct UniformBuffer;
struct Shader;
struct Texture2D;
struct TextureArray2D;
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
	std::vector<std::shared_ptr<Texture2D>> texture_store;
	std::vector<std::shared_ptr<TextureArray2D>> sprite_atlas_store;

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

enum VertexArrayAttrib {
	position = 0,
	normal,
	color,
	uv0,
	uv1,
	uv2,
	uv3,
	uv4,
	uv5,
	uv6,
	uv7,

	VertexArrayAttrib_COUNT
};

struct VertexArrayAttribDescriptor {
	VertexArrayAttrib attr_type;
	unsigned int idx;
	unsigned int size;
	unsigned int type;

	VertexArrayAttribDescriptor(VertexArrayAttrib attr_type, unsigned int idx, unsigned int size, unsigned int type) {
		this->idx = idx;
		this->size = size;
		this->type = type;
		this->attr_type = attr_type;
	}

	// VertexArrayAttribDescriptor(VertexArrayAttrib attr_type, unsigned int idx, uint32_t type) {
	// 	// this->idx = idx;
	// 	// this->size = size;
	// 	this->type = type;
	// 	this->attr_type = attr_type;
	// }
};

struct VertexArray {
	VertexArrayId id;
	std::size_t stride = 0;
	uint32_t elem_stride = 0;

	uint32_t position_offset = 0;
	bool has_position = false;
	uint32_t normal_offset = 0;
	bool has_normal = false;
	uint32_t color_offset = 0;
	bool has_color = false;
	uint32_t uv0_offset = 0;
	bool has_uv0 = false;
	uint32_t uv1_offset = 0;
	bool has_uv1 = false;
	uint32_t uv2_offset = 0;
	bool has_uv2 = false;
	uint32_t uv3_offset = 0;
	bool has_uv3 = false;
	uint32_t uv4_offset = 0;
	bool has_uv4 = false;
	uint32_t uv5_offset = 0;
	bool has_uv5 = false;
	uint32_t uv6_offset = 0;
	bool has_uv6 = false;
	uint32_t uv7_offset = 0;
	bool has_uv7 = false;
	
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

	bool SetUniformi(std::string uniform, int v0);
	bool SetUniformf(std::string uniform, float v0);
	bool SetUniformVec3(std::string uniform, const float* v);
	bool SetUniformVec4(std::string uniform, const float* v);
	bool SetUniformMat4(std::string uniform, const float* matrix);
	bool SetUniformArray(std::string uniform, std::size_t count, const float* v);
};

struct Texture2D {
	TextureId id;
	std::string type;
	std::string filename;
	uint32_t width, height, channels;

	void Bind();
	void BindUnit(uint32_t unit);
	void UnBind();
};

struct TextureArray2D {
	Texture2D* texture = nullptr;

	int tilecount;
	int tileheight;
	int tilewidth;
	int columns;
	
	void Bind();
	void UnBind();
};

void OpenGL_CreateContext();
void OpenGL_DestroyContext();

FrameBuffer* FrameBuffer_Create(int width, int height, bool picking = false);

VertexArray* VertexArray_Create(std::vector<VertexArrayAttribDescriptor> layouts);

VertexBuffer* VertexBuffer_Create();

IndexBuffer* IndexBuffer_Create(VertexArray* vertexarray);

UniformBuffer* UniformBuffer_Create();

Shader* Shader_Create(std::string name, const std::string &vs_filename, const std::string &fs_filename, bool file = true);

Texture2D* Texture_Create();
Texture2D* Texture_LoadFile(const char* filename);
TextureArray2D* TextureArray_LoadFile(int tilew, int tileh, const char* filename);
