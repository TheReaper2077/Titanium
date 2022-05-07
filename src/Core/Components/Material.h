#pragma once

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_cross_product.hpp>
#include <OpenGL.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Enumerations.h"

#include "../uuid.h"

// #define MAT_AMBIENT_COLOR_BIT (1 << ti::MaterialAttrib::Ambient_Color)
// #define MAT_DIFFUSE_COLOR_BIT (1 << ti::MaterialAttrib::Diffuse_Color)
// #define MAT_SPECULAR_COLOR_BIT (1 << ti::MaterialAttrib::Specular_Color)
#define MAT_AMBIENT_TEXTURE_BIT (1 << ti::MaterialAttrib::Ambient_Texture)
#define MAT_DIFFUSE_TEXTURE_BIT (1 << ti::MaterialAttrib::Diffuse_Texture)
#define MAT_SPECULAR_TEXTURE_BIT (1 << ti::MaterialAttrib::Specular_Texture)
// #define MAT_SHININESS_FLOAT_BIT (1 << ti::MaterialAttrib::Shininess_Float)

namespace ti {

	namespace Component {
		struct Material {
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
			float shininess;

			// std::string id = uuids::to_string(uuids::uuid_system_generator{}());

			Texture2D* ambient_map = nullptr;
			Texture2D* diffuse_map = nullptr;
			Texture2D* specular_map = nullptr;

			float roughness = 0.5;
			float metallic = 0;
			glm::vec3 albedo = glm::vec3(1, 0, 0);

			// Shader* shader;
			std::string name;

			uint32_t flags = 0;

			Material() {}
			Material(aiMaterial *ai_material) {
				aiString str;
				for (int i = 0; i < ai_material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
					ai_material->GetTexture(aiTextureType_DIFFUSE, i, &str);

					this->diffuse_map = Texture_LoadFile(str.C_Str());
					flags |= MAT_DIFFUSE_TEXTURE_BIT;
					break;
				}

				for (int i = 0; i < ai_material->GetTextureCount(aiTextureType_AMBIENT); i++) {
					ai_material->GetTexture(aiTextureType_AMBIENT, i, &str);

					this->ambient_map = Texture_LoadFile(str.C_Str());
					flags |= MAT_AMBIENT_TEXTURE_BIT;
					break;
				}

				for (int i = 0; i < ai_material->GetTextureCount(aiTextureType_SPECULAR); i++) {
					ai_material->GetTexture(aiTextureType_SPECULAR, i, &str);

					this->specular_map = Texture_LoadFile(str.C_Str());
					flags |= MAT_SPECULAR_TEXTURE_BIT;
					break;
				}

				name = std::string(ai_material->GetName().C_Str());

				aiColor4D color;
				
				if (AI_SUCCESS == aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_AMBIENT, &color)) {
					this->ambient = glm::vec3(color.r, color.g, color.b);
					// flags |= MAT_AMBIENT_COLOR_BIT;
				}
				
				if (AI_SUCCESS == aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_DIFFUSE, &color)) {
					this->diffuse = glm::vec3(color.r, color.g, color.b);
					// flags |= MAT_DIFFUSE_COLOR_BIT;
				}

				if (AI_SUCCESS == aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_SPECULAR, &color)) {
					this->specular = glm::vec3(color.r, color.g, color.b);
					// flags |= MAT_SPECULAR_COLOR_BIT;
				}

				if (AI_SUCCESS != aiGetMaterialFloat(ai_material, AI_MATKEY_SHININESS, &shininess)) {
					this->shininess = 20.f;
					// flags |= MAT_SHININESS_FLOAT_BIT;
				}
			}
		};
	}
}