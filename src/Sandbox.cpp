#include "Sandbox.h"
#include "Core/System/RenderSystem.h"
#include "Core/System/CameraSystem.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

ti::System::RenderSystem rendersystem;
ti::System::CameraSystem camerasystem;
Assimp::Importer importer;

void process_node(ti::ECS::Registry* registry, const aiScene *ai_scene, aiNode *ai_node) {
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
		process_node(registry, ai_scene, ai_node->mChildren[i]);
	}
};

void Sandbox::Init() {
	auto& engine = registry->Store<ti::EngineProperties>();

	auto camera = registry->Create();
	registry->Add<ti::Component::Camera>(camera, ti::Projection::PERSPECTIVE, true);
	registry->Add<ti::Component::Transform>(camera);
	registry->Add<ti::Component::Tag>(camera, "cam0", camera);

	rendersystem.registry = registry;
	camerasystem.registry = registry;
	rendersystem.Init();
	camerasystem.Init();
	
	const aiScene *ai_scene = importer.ReadFile("D:\\C++\\2.5D Engine\\res\\cube.obj", aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
	assert(ai_scene && !(ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && ai_scene->mRootNode);

	process_node(registry, ai_scene, ai_scene->mRootNode);

	for (int i = 0; i < ai_scene->mNumMaterials; i++)
		registry->Store<ti::MaterialRegistry>().RegisterMaterial(ti::Component::Material(ai_scene->mMaterials[i]));

	// SDL_ShowCursor(SDL_DISABLE);
}

void Sandbox::Update(double dt) {
	auto& engine = registry->Store<ti::EngineProperties>();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glViewport(0, 0, engine.width, engine.height);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	camerasystem.Update(dt);
	rendersystem.Update(dt);
}
