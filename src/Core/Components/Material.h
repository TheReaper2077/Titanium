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

namespace ti {
	namespace Component {
		struct Material {
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
			float shininess;

			Texture2D* ambient_map;
			Texture2D* diffuse_map;
			Texture2D* specular_map;

			// Shader* shader;
			std::string name;

			Material() {}
			Material(aiMaterial *ai_material) {
				aiString str;
				for (int i = 0; i < ai_material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
					ai_material->GetTexture(aiTextureType_DIFFUSE, i, &str);

					this->diffuse_map = Texture_LoadFile(str.C_Str());
					break;
				}

				for (int i = 0; i < ai_material->GetTextureCount(aiTextureType_AMBIENT); i++) {
					ai_material->GetTexture(aiTextureType_AMBIENT, i, &str);

					this->ambient_map = Texture_LoadFile(str.C_Str());
					break;
				}

				for (int i = 0; i < ai_material->GetTextureCount(aiTextureType_SPECULAR); i++) {
					ai_material->GetTexture(aiTextureType_SPECULAR, i, &str);

					this->specular_map = Texture_LoadFile(str.C_Str());
					break;
				}

				name = std::string(ai_material->GetName().C_Str());

				// aiColor3D color(0.f, 0.f, 0.f);
				// if (AI_SUCCESS == aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_AMBIENT, color))
				// 	this->ambient = glm::vec3(color.r, color.g, color.b);
				// if (AI_SUCCESS == aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_DIFFUSE, color))
				// 	this->diffuse = glm::vec3(color.r, color.g, color.b);
				// if (AI_SUCCESS == aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_SPECULAR, color))
				// 	this->specular = glm::vec3(color.r, color.g, color.b);
			}
		};
	}
}