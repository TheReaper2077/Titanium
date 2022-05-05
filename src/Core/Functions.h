#pragma once

#include "ECS.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace ti {
	struct Functions {
		ti::ECS::Registry* registry;
		Assimp::Importer* importer;

		ti::ECS::Entity editor_camera = 0;

		std::unordered_map<std::size_t, ECS::Entity> entities;

		bool runtime_creation = false;
		bool save_entities = false;

		Functions();
		Functions(ti::ECS::Registry* registry);

	private:
		void process_node(const aiScene *ai_scene, aiNode *ai_node);

	public:
		void ImportModel(std::string filename, uint32_t flags = 0);
		void ImportModel(const aiScene* ai_scene);
		void ImportMaterials(const aiScene* ai_scene);
		
		void GetName();

		ti::ECS::Entity AddEmptyEntity();
		ti::ECS::Entity AddMeshEntity();
		ti::ECS::Entity AddLightEntity();
		ti::ECS::Entity AddCameraEntity();
		ti::ECS::Entity AddSpriteEntity();

		void DuplicateEntity(ti::ECS::Entity entity);
		
		void SaveEntities();
		void LoadEntities();
	};
}