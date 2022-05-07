#include "Functions.h"
#include "Components/Components.h"
#include "uuid.h"
#include "MeshRegistry.h"
#include "MaterialRegistry.h"
#include "Serializer.h"

ti::Functions::Functions() {
	importer = new Assimp::Importer();
}

ti::Functions::Functions(ti::ECS::Registry* registry): registry(registry) {
	importer = new Assimp::Importer();
}

void ti::Functions::process_node(const aiScene *ai_scene, aiNode *ai_node) {
	for (int i = 0; i < ai_node->mNumMeshes; i++) {
		auto* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[i]];
		auto mesh_name = std::string(ai_mesh->mName.C_Str());

		registry->Store<MeshRegistry>().AddMesh(mesh_name, ti::Component::Mesh{ai_scene, ai_mesh});
	}

	for (int i = 0; i < ai_node->mNumChildren; i++) {
		process_node(ai_scene, ai_node->mChildren[i]);
	}
};

void ti::Functions::ImportModel(std::string filename, uint32_t flags) {
	ImportModel(importer->ReadFile(filename.c_str(), flags | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals));
}

void ti::Functions::ImportModel(const aiScene* ai_scene) {
	assert(ai_scene && !(ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && ai_scene->mRootNode);
	
	process_node(ai_scene, ai_scene->mRootNode);
	ImportMaterials(ai_scene);
}

void ti::Functions::ImportMaterials(const aiScene* ai_scene) {
	for (int i = 0; i < ai_scene->mNumMaterials; i++)
		registry->Store<ti::MaterialRegistry>().RegisterMaterial(ti::Component::Material(ai_scene->mMaterials[i]));
}

std::string ti::Functions::GetName(std::string name) {
	int i = 0;
	for (auto entity: registry->View<ti::Component::Tag>()) {
		if (registry->Get<ti::Component::Tag>(entity).name == name)
			i++;
	}

	if (i) {
		return name + "(" + std::to_string(i) + ")";
	}

	return name;
}

ti::ECS::Entity ti::Functions::AddEmptyEntity() {
	auto entity = registry->Create();
	
	registry->Add<ti::Component::Tag>(entity, GetName("Empty"), uuids::to_string(uuids::uuid_system_generator{}()), !runtime_creation);
	registry->Add<ti::Component::Transform>(entity);
	if (!runtime_creation) save_entities = true;

	return entity;
}

ti::ECS::Entity ti::Functions::AddMeshEntity() {
	auto entity = registry->Create();
	
	registry->Add<ti::Component::Tag>(entity, GetName("Mesh"), uuids::to_string(uuids::uuid_system_generator{}()), !runtime_creation);
	registry->Add<ti::Component::Transform>(entity);
	registry->Add<ti::Component::MeshFilter>(entity);
	registry->Add<ti::Component::MeshRenderer>(entity);
	if (!runtime_creation) save_entities = true;

	return entity;
}

ti::ECS::Entity ti::Functions::AddLightEntity() {
	auto entity = registry->Create();
	
	registry->Add<ti::Component::Tag>(entity, GetName("Light"), uuids::to_string(uuids::uuid_system_generator{}()), !runtime_creation);
	registry->Add<ti::Component::Transform>(entity);
	registry->Add<ti::Component::Light>(entity);
	if (!runtime_creation) save_entities = true;

	return entity;
}

ti::ECS::Entity ti::Functions::AddCameraEntity() {
	auto entity = registry->Create();
	
	registry->Add<ti::Component::Tag>(entity, GetName("Camera"), uuids::to_string(uuids::uuid_system_generator{}()), !runtime_creation);
	registry->Add<ti::Component::Transform>(entity);
	registry->Add<ti::Component::Camera>(entity, ti::Projection::PERSPECTIVE, false);
	if (!runtime_creation) save_entities = true;

	return entity;
}

ti::ECS::Entity ti::Functions::AddSpriteEntity() {
	auto entity = registry->Create();

	registry->Add<ti::Component::Tag>(entity, GetName("Sprite"), uuids::to_string(uuids::uuid_system_generator{}()), !runtime_creation);
	registry->Add<ti::Component::Transform>(entity);
	registry->Add<ti::Component::SpriteRenderer>(entity);
	if (!runtime_creation) save_entities = true;

	return entity;
}

void ti::Functions::DuplicateEntity(ti::ECS::Entity entity) {
	auto created_entity = registry->Create();

	registry->Add<ti::Component::Tag>(created_entity, registry->Get<ti::Component::Tag>(entity).name, uuids::to_string(uuids::uuid_system_generator{}()), !runtime_creation);
	registry->Add<ti::Component::Transform>(created_entity, registry->Get<ti::Component::Transform>(entity));
	
	if (registry->Contains<ti::Component::Camera>(entity))
		registry->Add<ti::Component::Camera>(created_entity, registry->Get<ti::Component::Camera>(entity));
	
	if (registry->Contains<ti::Component::Light>(entity))
		registry->Add<ti::Component::Light>(created_entity, registry->Get<ti::Component::Light>(entity));
	
	if (registry->Contains<ti::Component::MeshFilter>(entity))
		registry->Add<ti::Component::MeshFilter>(created_entity, registry->Get<ti::Component::MeshFilter>(entity));
	
	if (registry->Contains<ti::Component::SpriteRenderer>(entity))
		registry->Add<ti::Component::SpriteRenderer>(created_entity, registry->Get<ti::Component::SpriteRenderer>(entity));
	
	if (registry->Contains<ti::Component::MeshRenderer>(entity))
		registry->Add<ti::Component::MeshRenderer>(created_entity, registry->Get<ti::Component::MeshRenderer>(entity));
}

void ti::Functions::SaveEntities() {
	SerializeEntities(registry, "D:\\C++\\2.5D Engine\\assets\\scene.yaml");
}

void ti::Functions::LoadEntities() {
	DeSerializeEntities(registry, "D:\\C++\\2.5D Engine\\assets\\scene.yaml");
}
