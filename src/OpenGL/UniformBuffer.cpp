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

void UniformBuffer_Allocate(UniformBuffer *uniformbuffer, std::size_t size) {
	if (uniformbuffer->size > size) return;

	UniformBuffer_Bind(uniformbuffer);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	uniformbuffer->size = size;
};

void UniformBuffer_AddDataDynamic(UniformBuffer *uniformbuffer, void* data, std::size_t size, std::size_t offset) {
	assert(offset + size <= uniformbuffer->size);

	UniformBuffer_Bind(uniformbuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void UniformBuffer_BindRange(UniformBuffer *uniformbuffer, unsigned int index, std::size_t size, std::size_t offset) {
	UniformBuffer_Bind(uniformbuffer);
	glBindBufferRange(GL_UNIFORM_BUFFER, index, uniformbuffer->id, offset, size);
}

void UniformBuffer_Bind(UniformBuffer *uniformbuffer) {
	if (gl_context->binding_uniformbuffer == uniformbuffer->id) return;
	gl_context->binding_uniformbuffer = uniformbuffer->id;
	glBindBuffer(GL_UNIFORM_BUFFER, uniformbuffer->id);
}

void UniformBuffer_UnBind() {
	gl_context->binding_uniformbuffer = 0;
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}