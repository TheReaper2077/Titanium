#include <OpenGL.h>

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

void IndexBuffer::Bind() {
	this->vertexarray->Bind();
	if (gl_context->binding_indexbuffer == this->id) return;
	gl_context->binding_indexbuffer = this->id;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
}

void IndexBuffer::UnBind() {
	gl_context->binding_indexbuffer = 0;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void IndexBuffer::Destroy() {
	glDeleteBuffers(1, &this->id);
	// free(indexbuffer);
	// indexbuffer = nullptr;
}

void IndexBuffer::Allocate(std::size_t size) {
	if (this->size > size) return;

	this->Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	this->size = size;
};

void IndexBuffer::AddData(unsigned int* data, std::size_t size, std::size_t offset) {
	if (size == 0) return;

	if (offset == 0 && size > this->size) {
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
		this->size = size;
		return;
	}

	assert(offset + size <= this->size);

	Bind();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	this->size = size;
}
