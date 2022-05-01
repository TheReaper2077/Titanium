#pragma once

#include "ECS.h"

#include "Components/Components.h"

#include "MaterialRegistry.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace ti {
	struct Functions {
		ti::ECS::Registry* registry;
		Assimp::Importer* importer;

		Functions() {
			importer = new Assimp::Importer();
		}
		Functions(ti::ECS::Registry* registry): registry(registry) {
			importer = new Assimp::Importer();
		}

	private:
		void process_node(const aiScene *ai_scene, aiNode *ai_node) {
			for (int i = 0; i < ai_node->mNumMeshes; i++) {
				auto* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[i]];
				auto mesh_name = std::string(ai_mesh->mName.C_Str());

				auto entity = registry->Create();
				registry->Add<ti::Component::Tag>(entity, mesh_name, entity);
				registry->Add<ti::Component::Transform>(entity);
				registry->Add<ti::Component::Mesh>(entity, ai_scene, ai_mesh);
				registry->Add<ti::Component::MeshRenderer>(entity, ai_scene, ai_mesh);
			}

			for (int i = 0; i < ai_node->mNumChildren; i++) {
				process_node(ai_scene, ai_node->mChildren[i]);
			}
		};

	public:
		void ImportModel(std::string filename, uint32_t flags = 0) {
			ImportModel(importer->ReadFile(filename.c_str(), flags | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals));
		}

		void ImportModel(const aiScene* ai_scene) {
			assert(ai_scene && !(ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && ai_scene->mRootNode);
			
			process_node(ai_scene, ai_scene->mRootNode);
			ImportMaterials(ai_scene);
		}

		void ImportMaterials(const aiScene* ai_scene) {
			for (int i = 0; i < ai_scene->mNumMaterials; i++)
				registry->Store<ti::MaterialRegistry>().RegisterMaterial(ti::Component::Material(ai_scene->mMaterials[i]));
		}

		
	};
}