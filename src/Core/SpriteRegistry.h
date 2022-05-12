#pragma once

#include "Components/Components.h"
#include <unordered_map>
#include "uuid.h"

namespace ti {
	struct SpriteRegistry {
		std::unordered_map<std::string, ti::Component::Texture2D> filename_texture_store;
		std::unordered_map<std::string, std::string> id_filename_store;

		void AddTexture(std::string filename) {
			auto texture = ti::Component::Texture2D(filename);
			
			texture.id = uuids::to_string(uuids::uuid_system_generator{}());

			filename_texture_store[filename] = texture;
		}

		ti::Component::Texture2D& GetTextureFilename(std::string filename) {
			assert(filename_texture_store.find(filename) == filename_texture_store.end());

			return filename_texture_store[filename];
		}

		ti::Component::Texture2D& GetTextureID(std::string id) {
			assert(id_filename_store.find(id) == id_filename_store.end());

			return filename_texture_store[id_filename_store[id]];
		}
	};
}