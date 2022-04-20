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

void VertexBuffer::Bind() {
	if (gl_context->binding_vertexbuffer == this->id) return;
	gl_context->binding_vertexbuffer = this->id;
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
}

void VertexBuffer::UnBind() {
	gl_context->binding_vertexbuffer = 0;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Destroy() {
	glDeleteBuffers(1, &this->id);
	// free(vertexbuffer);
	// vertexbuffer = nullptr;
}

void VertexBuffer::Allocate(std::size_t size) {
	if (this->size > size && size == 0) return;

	Bind();
	if (this->type == DYNAMIC) glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	if (this->type == STATIC) glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
	this->size = size;
};

void VertexBuffer::AddDataStatic(void* data, std::size_t size) {
	if (size == 0) return;
	Bind();
	if (this->type == DYNAMIC) glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	if (this->type == STATIC) glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	this->size = size;
}

void VertexBuffer::AddDataDynamic(void* data, std::size_t size, std::size_t offset) {
	if (size == 0) return;
	assert(offset + size <= this->size);

	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	this->size = size;
}
