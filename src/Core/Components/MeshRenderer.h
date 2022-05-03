#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_cross_product.hpp>
#include <OpenGL.h>
#include "../Enumerations.h"

namespace ti {
	namespace Component {
		struct MeshRenderer {
			std::string material;
			bool lighting_enable = true;

			VertexArray* vertexarray = nullptr;
			uint32_t flags = POSITION_ATTRIB_BIT;

			MeshRenderer() {}
			MeshRenderer(const aiScene* ai_scene, aiMesh* ai_mesh) {
				if (ai_mesh->mMaterialIndex >= 0)
					material = std::string(ai_scene->mMaterials[ai_mesh->mMaterialIndex]->GetName().C_Str());
			}
		};
	}
}