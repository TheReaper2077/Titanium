#pragma once

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_cross_product.hpp>

#include "Mesh.h"

#include "assert.h"

namespace ti {
	namespace Component {
		struct Model {
			std::vector<Mesh> meshes;

			Model() {}
			Model(const aiScene *ai_scene) {
				assert(ai_scene && !(ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && ai_scene->mRootNode);
				
				process_node(ai_scene, ai_scene->mRootNode);
			}

			void process_mesh(const aiScene *ai_scene, aiMesh *ai_mesh) {
				Mesh mesh;

				for (int i = 0; i < ai_mesh->mNumVertices; i++) {
					Vertex vertex;

					vertex.position.x = ai_mesh->mVertices[i].x;
					vertex.position.y = ai_mesh->mVertices[i].y;
					vertex.position.z = ai_mesh->mVertices[i].z;

					vertex.normal.x = ai_mesh->mNormals[i].x;
					vertex.normal.y = ai_mesh->mNormals[i].y;
					vertex.normal.z = ai_mesh->mNormals[i].z;

					if (ai_mesh->mTextureCoords[0]) {
						vertex.uv.x = ai_mesh->mTextureCoords[0][i].x;
						vertex.uv.y = ai_mesh->mTextureCoords[0][i].y;
						vertex.uv.z = 0;
					}

					mesh.vertices.push_back(vertex);
				}

				for (int i = 0; i < ai_mesh->mNumFaces; i++) {
					aiFace face = ai_mesh->mFaces[i];

					for(unsigned int j = 0; j < face.mNumIndices; j++) {
						mesh.indices.push_back(face.mIndices[j]);
					}
				}

				if (ai_mesh->mMaterialIndex >= 0) {
					mesh.material = std::string(ai_scene->mMaterials[ai_mesh->mMaterialIndex]->GetName().C_Str());
				}

				meshes.push_back(mesh);
			}

			void process_node(const aiScene *ai_scene, aiNode *ai_node) {
				for (int i = 0; i < ai_node->mNumMeshes; i++) {
					process_mesh(ai_scene, ai_scene->mMeshes[ai_node->mMeshes[i]]);
				}

				for (int i = 0; i < ai_node->mNumChildren; i++) {
					process_node(ai_scene, ai_node->mChildren[i]);
				}
			}
		};
	}
}