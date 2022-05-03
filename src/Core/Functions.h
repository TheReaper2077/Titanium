#pragma once

#include "ECS.h"

#include "Components/Components.h"

#include "MeshRegistry.h"
#include "MaterialRegistry.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace ti {
	struct Functions {
		ti::ECS::Registry* registry;
		Assimp::Importer* importer;

		ti::ECS::Entity editor_camera = 0;

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

				registry->Store<MeshRegistry>().AddMesh(mesh_name, ti::Component::Mesh{ai_scene, ai_mesh});
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

		ti::ECS::Entity AddEmptyEntity() {
			auto entity = registry->Create();
			
			registry->Add<ti::Component::Tag>(entity, "Empty - " + std::to_string(entity), entity);
			registry->Add<ti::Component::Transform>(entity);

			return entity;
		}

		ti::ECS::Entity AddMeshEntity() {
			auto entity = registry->Create();
			
			registry->Add<ti::Component::Tag>(entity, "Mesh - " + std::to_string(entity), entity);
			registry->Add<ti::Component::Transform>(entity);
			registry->Add<ti::Component::MeshFilter>(entity);
			registry->Add<ti::Component::MeshRenderer>(entity);

			return entity;
		}

		ti::ECS::Entity AddLightEntity() {
			auto entity = registry->Create();
			
			registry->Add<ti::Component::Tag>(entity, "Light - " + std::to_string(entity), entity);
			registry->Add<ti::Component::Transform>(entity);
			registry->Add<ti::Component::Light>(entity);

			return entity;
		}

		ti::ECS::Entity AddCameraEntity() {
			auto entity = registry->Create();
			
			registry->Add<ti::Component::Tag>(entity, "Camera - " + std::to_string(entity), entity);
			registry->Add<ti::Component::Transform>(entity);
			registry->Add<ti::Component::Camera>(entity, ti::Projection::PERSPECTIVE, false);

			return entity;
		}

		ti::ECS::Entity AddSpriteEntity() {
			auto entity = registry->Create();

			registry->Add<ti::Component::Tag>(entity, "Camera - " + std::to_string(entity), entity);
			registry->Add<ti::Component::Transform>(entity);
			registry->Add<ti::Component::SpriteRenderer>(entity);

			return entity;
		}
	};
}