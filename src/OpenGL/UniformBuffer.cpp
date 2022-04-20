#include "OpenGL.h"

extern std::shared_ptr<OpenGLContext> gl_context;


UniformBuffer *UniformBuffer_Create() {
	assert(gl_context != nullptr);

	auto uniformbuffer = std::make_shared<UniformBuffer>();

	glGenBuffers(1, &uniformbuffer->id);
	uniformbuffer->size = 0;

	gl_context->uniform_buffer_store.push_back(uniformbuffer);

	return uniformbuffer.get();
}

void UniformBuffer::Allocate(std::size_t size) {
	if (this->size > size) return;

	Bind();
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	this->size = size;
};

void UniformBuffer::AddDataDynamic(void* data, std::size_t size, std::size_t offset) {
	assert(offset + size <= this->size);

	Bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void UniformBuffer::BindRange(unsigned int index, std::size_t size, std::size_t offset) {
	Bind();
	glBindBufferRange(GL_UNIFORM_BUFFER, index, this->id, offset, size);
}

void UniformBuffer::Bind() {
	if (gl_context->binding_uniformbuffer == this->id) return;
	gl_context->binding_uniformbuffer = this->id;
	glBindBuffer(GL_UNIFORM_BUFFER, this->id);
}

void UniformBuffer::UnBind() {
	gl_context->binding_uniformbuffer = 0;
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}