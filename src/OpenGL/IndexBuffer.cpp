#include "OpenGL.h"

extern std::shared_ptr<OpenGLContext> gl_context;

IndexBuffer *IndexBuffer_Create(VertexArray *vertexarray) {
	assert(gl_context != nullptr);

	auto indexbuffer = std::make_shared<IndexBuffer>();

	glGenBuffers(1, &indexbuffer->id);
	indexbuffer->size = 0;
	indexbuffer->vertexarray = vertexarray;

	gl_context->index_buffer_store.push_back(indexbuffer);

	return indexbuffer.get();
}

void IndexBuffer_Bind(IndexBuffer *indexbuffer) {
	VertexArray_Bind(indexbuffer->vertexarray);
	if (gl_context->binding_indexbuffer == indexbuffer->id) return;
	gl_context->binding_indexbuffer = indexbuffer->id;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer->id);
}

void IndexBuffer_UnBind() {
	gl_context->binding_indexbuffer = 0;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void IndexBuffer_Destroy(IndexBuffer *indexbuffer) {
	glDeleteBuffers(1, &indexbuffer->id);
	free(indexbuffer);
	indexbuffer = nullptr;
}

void IndexBuffer_Allocate(IndexBuffer *indexbuffer, std::size_t size) {
	if (indexbuffer->size > size) return;

	IndexBuffer_Bind(indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	indexbuffer->size = size;
};

void IndexBuffer_AddData(IndexBuffer *indexbuffer, unsigned int* data, std::size_t size, std::size_t offset) {
	if (offset == 0 && size > indexbuffer->size) {
		IndexBuffer_Bind(indexbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
		indexbuffer->size = size;
		return;
	}

	assert(offset + size <= indexbuffer->size);

	IndexBuffer_Bind(indexbuffer);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	indexbuffer->size = size;
}
