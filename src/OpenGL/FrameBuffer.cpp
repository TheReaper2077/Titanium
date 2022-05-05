#pragma once

#include <OpenGL.h>

extern std::shared_ptr<OpenGLContext> gl_context;

FrameBuffer* FrameBuffer_Create(int width, int height, bool picking) {
	auto framebuffer = std::make_shared<FrameBuffer>();

	framebuffer->width = width;
	framebuffer->height = height;

	glGenFramebuffers(1, &framebuffer->id);

	framebuffer->Bind();

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (picking) {
		unsigned int pickingtexture;
		glGenTextures(1, &pickingtexture);
		glBindTexture(GL_TEXTURE_2D, pickingtexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, pickingtexture, 0);
	}

	unsigned int depth_stencil;
	glGenTextures(1, &depth_stencil);
	glBindTexture(GL_TEXTURE_2D, depth_stencil);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
	
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_stencil, 0);

	GL_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	framebuffer->UnBind();

	gl_context->frame_buffer_store.push_back(framebuffer);

	return framebuffer.get();
}

void FrameBuffer::Bind() {
	if (gl_context->binding_framebuffer == this->id) return;
	gl_context->binding_framebuffer = this->id;
	glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}

void FrameBuffer::UnBind() {
	gl_context->binding_framebuffer = 0;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
