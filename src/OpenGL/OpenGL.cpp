
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "OpenGL.h"

std::shared_ptr<OpenGLContext> gl_context;

void OpenGL_CreateContext() {
	GL_ASSERT(gl_context == nullptr);

	gl_context = std::make_shared<OpenGLContext>();
}

void OpenGL_DestroyContext() {
	GL_ASSERT(gl_context != nullptr);
}
