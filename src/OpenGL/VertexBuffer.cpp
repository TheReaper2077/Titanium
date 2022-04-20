#pragma once

#include "OpenGL.h"

extern std::shared_ptr<OpenGLContext> gl_context;

VertexBuffer *VertexBuffer_Create() {
	assert(gl_context != nullptr);

	auto vertexbuffer = std::make_shared<VertexBuffer>();

	glGenBuffers(1, &vertexbuffer->id);
	vertexbuffer->type = DYNAMIC;
	vertexbuffer->size = 0;

	gl_context->vertex_buffer_store.push_back(vertexbuffer);

	return vertexbuffer.get();
}

void VertexBuffer_Bind(VertexBuffer *vertexbuffer) {
	if (gl_context->binding_vertexbuffer == vertexbuffer->id) return;
	gl_context->binding_vertexbuffer = vertexbuffer->id;
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer->id);
}

void VertexBuffer_UnBind() {
	gl_context->binding_vertexbuffer = 0;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer_Destroy(VertexBuffer *vertexbuffer) {
	glDeleteBuffers(1, &vertexbuffer->id);
	free(vertexbuffer);
	vertexbuffer = nullptr;
}

void VertexBuffer_Allocate(VertexBuffer *vertexbuffer, std::size_t size) {
	if (vertexbuffer->size > size && size == 0) return;

	VertexBuffer_Bind(vertexbuffer);
	if (vertexbuffer->type == DYNAMIC) glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	if (vertexbuffer->type == STATIC) glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
	vertexbuffer->size = size;
};

void VertexBuffer_AddDataStatic(VertexBuffer *vertexbuffer, void* data, std::size_t size) {
	if (size == 0) return;
	VertexBuffer_Bind(vertexbuffer);
	if (vertexbuffer->type == DYNAMIC) glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	if (vertexbuffer->type == STATIC) glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	vertexbuffer->size = size;
}

void VertexBuffer_AddDataDynamic(VertexBuffer *vertexbuffer, void* data, std::size_t size, std::size_t offset) {
	if (size == 0) return;
	assert(offset + size <= vertexbuffer->size);

	VertexBuffer_Bind(vertexbuffer);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	vertexbuffer->size = size;
}
