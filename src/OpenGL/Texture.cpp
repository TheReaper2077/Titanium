#include <OpenGL.h>

#include <stb_image.h>

extern std::shared_ptr<OpenGLContext> gl_context;

Texture2D *Texture_Create() {
	assert(gl_context != nullptr);

	auto texture = std::make_shared<Texture2D>();
	glGenTextures(1, &texture->id);
	
	gl_context->texture_store.push_back(texture);

	return texture.get();
}

Texture2D *Texture_LoadFile(const char* filename) {
	assert(gl_context != nullptr);

	auto* texture = Texture_Create();
	
	texture->Bind();
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenerateMipmap(texture->id);

	int width, height, channels;
	unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);

	assert(data);

	if (channels == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	if (channels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	stbi_image_free(data);

	texture->UnBind();

	texture->channels = channels;
	texture->width = width;
	texture->height = height;

	return texture;
}

void Texture2D::Bind() {
	glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture2D::BindUnit(uint32_t unit) {
	glBindTextureUnit(unit, this->id);
}

void Texture2D::UnBind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
