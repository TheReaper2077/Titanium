#include <OpenGL.h>

extern std::shared_ptr<OpenGLContext> gl_context;

// experimental Haven't Tested
TextureArray *TextureArray_LoadFile(int tilew, int tileh, const char* filename) {
	assert(gl_context != nullptr);

	auto spriteatlas = std::make_shared<TextureArray>();

	spriteatlas->texture = Texture_Create();
	spriteatlas->texture->Bind();
	
	int width, height, channels;
	auto* data = stbi_load(filename, &width, &height, &channels, 0);

	spriteatlas->texture->width = width;
	spriteatlas->texture->height = height;
	spriteatlas->texture->channels = channels;
	
	int tilex = spriteatlas->texture->width/tilew;
	int tiley = spriteatlas->texture->height/tileh;

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, tilew, tileh, tilex * tiley, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int offset = 0;
	unsigned char* tmp = (unsigned char*)alloca(tileh*tilew*channels);

	for (int y = 0; y != tiley; y++) {
		for (int x = 0; x != tilex; x++) {
			for (int tx = 0; tx != tilew; tx++) {
				for (int ty = 0; ty != tileh; ty++) {
					tmp[channels*tileh*ty + channels*tx + 0] = data[channels*width*tileh*y + channels*tilew*x + channels*width*ty + channels*tx + 0];	// r
					tmp[channels*tileh*ty + channels*tx + 1] = data[channels*width*tileh*y + channels*tilew*x + channels*width*ty + channels*tx + 1];	// b
					tmp[channels*tileh*ty + channels*tx + 2] = data[channels*width*tileh*y + channels*tilew*x + channels*width*ty + channels*tx + 2];	// g
					if (channels == 4)
					tmp[channels*tileh*ty + channels*tx + 3] = data[channels*width*tileh*y + channels*tilew*x + channels*width*ty + channels*tx + 3];	// a
				}
			}

			switch (channels) {
				case 4:
					glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, offset, tilew, tileh, 1, GL_RGBA, GL_UNSIGNED_BYTE, tmp);
					break;
					
				case 3:
					glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, offset, tilew, tileh, 1, GL_RGB, GL_UNSIGNED_BYTE, tmp);
					break;
				
				default:
					assert(false);
			}

			offset++;
		}
	}

	stbi_image_free(data);

	spriteatlas->texture->channels = channels;
	spriteatlas->tilecount = offset;
	spriteatlas->tileheight = tileh;
	spriteatlas->tilewidth = tilew;
	spriteatlas->columns = tilex;

	gl_context->sprite_atlas_store.push_back(spriteatlas);

	return spriteatlas.get();
}

void TextureArray::Bind() {
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->texture->id);
}

void TextureArray::UnBind() {
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}