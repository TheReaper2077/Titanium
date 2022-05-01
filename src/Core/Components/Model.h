#pragma once

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_cross_product.hpp>

#include "MeshFilter.h"

#include "assert.h"

// namespace ti {
// 	namespace Component {
// 		struct Model {
// 			std::vector<Mesh> meshes;

// 			Model() {}
// 			Model(const aiScene *ai_scene) {
// 				assert(ai_scene && !(ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && ai_scene->mRootNode);
				
// 				process_node(ai_scene, ai_scene->mRootNode);
// 			}

// 			void process_node(const aiScene *ai_scene, aiNode *ai_node) {
// 				for (int i = 0; i < ai_node->mNumMeshes; i++) {
// 					meshes.push_back(Mesh(ai_scene, ai_scene->mMeshes[ai_node->mMeshes[i]]));
// 				}

// 				for (int i = 0; i < ai_node->mNumChildren; i++) {
// 					process_node(ai_scene, ai_node->mChildren[i]);
// 				}
// 			}
// 		};
// 	}
// }