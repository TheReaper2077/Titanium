#pragma once

#include <OpenGL.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_cross_product.hpp>

namespace ti {
	namespace Component {
		struct Texture2D {
			unsigned int texture;
			std::string filename;
			std::string id;
			uint32_t width, height, channels;

			Texture2D() {}
			Texture2D(std::string filename) {
				auto* tex = Texture_LoadFile(filename.c_str());
				width = tex->width;
				height = tex->height;
				channels = tex->channels;
				this->filename = filename;
				texture = tex->id;
			}
		};
	}
}