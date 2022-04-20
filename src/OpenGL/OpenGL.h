#pragma once

#define GL_API

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
typedef unsigned int SpriteId;

// OpenGLContext
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

GL_API void OpenGL_CreateContext();
GL_API void OpenGL_DestroyContext();

GL_API FrameBuffer *FrameBuffer_Create();

// #ifndef VERTEX_ARRAY	// VertexArray


GL_API VertexArray *VertexArray_Create(std::vector<VertexArrayLayout> layouts);
GL_API void VertexArray_Bind(VertexArray *vertexarray);
GL_API void VertexArray_BindVertexBuffer(VertexArray *vertexarray, VertexBuffer *vertexbuffer, std::size_t stride = 0, std::size_t offset = 0);
GL_API void VertexArray_BindIndexBuffer(VertexArray *vertexarray, IndexBuffer *indexbuffer);
GL_API void VertexArray_UnBind();
GL_API void VertexArray_Destroy(VertexArray *vertexarray);

// #endif

// #ifndef VERTEX_BUFFER	// VertexBuffer


GL_API VertexBuffer *VertexBuffer_Create();
GL_API void VertexBuffer_Bind(VertexBuffer *vertexbuffer);
GL_API void VertexBuffer_UnBind();
GL_API void VertexBuffer_Allocate(VertexBuffer *vertexbuffer, std::size_t size);
GL_API void VertexBuffer_AddDataDynamic(VertexBuffer *vertexbuffer, void* data, std::size_t size, std::size_t offset = 0);
GL_API void VertexBuffer_AddDataStatic(VertexBuffer *vertexbuffer, void* data, std::size_t size);
GL_API void VertexBuffer_Destroy(VertexBuffer *vertexbuffer);

// #endif

// #ifndef INDEX_BUFFER	// IndexBuffer


GL_API IndexBuffer *IndexBuffer_Create(VertexArray *vertexarray);
GL_API void IndexBuffer_Bind(IndexBuffer *Indexbuffer);
GL_API void IndexBuffer_UnBind();
GL_API void IndexBuffer_Allocate(IndexBuffer *Indexbuffer, std::size_t size);
GL_API void IndexBuffer_AddData(IndexBuffer *indexbuffer, unsigned int* data, std::size_t size, std::size_t offset = 0);
GL_API void IndexBuffer_Destroy(IndexBuffer *Indexbuffer);

// #endif

// #ifndef UNIFORM_BUFFER


GL_API UniformBuffer *UniformBuffer_Create();
GL_API void UniformBuffer_Allocate(UniformBuffer *uniformbuffer, std::size_t size);
GL_API void UniformBuffer_AddDataDynamic(UniformBuffer *uniformbuffer, void* data, std::size_t size, std::size_t offset = 0);
GL_API void UniformBuffer_BindRange(UniformBuffer *uniformbuffer, unsigned int index, std::size_t size, std::size_t offset = 0);
GL_API void UniformBuffer_Bind(UniformBuffer *uniformbuffer);
GL_API void UniformBuffer_UnBind();

// #endif

// #ifndef SHADER	// Shader


GL_API Shader *Shader_Create(const std::string &vs_filename, const std::string &fs_filename, bool file = true);
GL_API void Shader_Bind(Shader *shader);
GL_API void Shader_UnBind();

GL_API void Shader_BindUniformBlock(Shader *shader, std::string name, unsigned int index);
GL_API unsigned int Shader_GetUniformLoc(Shader *shader, std::string name);

GL_API void Shader_SetUniformi(Shader *shader, std::string uniform, int v0);
GL_API void Shader_SetUniformf(Shader *shader, std::string uniform, float v0);
GL_API void Shader_SetUniformVec3(Shader *shader, std::string uniform, const float *v);
GL_API void Shader_SetUniformVec4(Shader *shader, std::string uniform, const float *v);
GL_API void Shader_SetUniformMat4(Shader *shader, std::string uniform, const float* matrix);
GL_API void Shader_SetUniformArray(Shader *shader, std::string uniform, std::size_t count, const float *v);

// Texture
GL_API Texture *Texture_Create();
GL_API Texture *Texture_LoadFile(const char* filename);
GL_API void Texture_Bind(Texture *texture);
GL_API void Texture_BindUnit(Texture *texture, uint32_t unit);
GL_API void Texture_UnBind();
GL_API void TextureArray_Bind(Texture *texture);
GL_API void TextureArray_UnBind();
GL_API TextureArray *TextureArray_LoadFile(int tilew, int tileh, const char* filename);

// #endif

#include "Impl.h"
