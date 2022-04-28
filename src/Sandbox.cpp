#include "Sandbox.h"
#include "Camera.h"
#include "Core/System/RenderSystem.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

ti::System::RenderSystem rendersystem;
ti::Camera* camera;
Assimp::Importer importer;

void Sandbox::Init() {
	camera = new ti::Camera("cam0", ti::PERSPECTIVE, width, height);

	const aiScene *scene = importer.ReadFile("D:\\C++\\2.5D Engine\\res\\cylinder.obj", aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
	assert(scene && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode);

	auto entity = registry->Create();
	registry->Add<ti::Component::Model>(entity, scene);
	registry->Add<ti::Component::Transform>(entity);
	registry->Add<ti::Component::Properties>(entity, "name", entity);

	rendersystem.registry = registry;

	rendersystem.Init();

	for (int i = 0; i < scene->mNumMaterials; i++)
		rendersystem.RegisterMaterial(ti::Component::Material(scene->mMaterials[i]));

	SDL_ShowCursor(SDL_DISABLE);
}

void Sandbox::Update(double dt) {
	glViewport(0, 0, this->width, this->height);
	glClearColor(0.2, 0.2, 0.2, 0.2);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	camera->Update(dt, eventdata);

	rendersystem.SetView(camera->view);
	// rendersystem.SetModel(camera->model);
	rendersystem.SetProjection(camera->projection);
	
	rendersystem.Update(dt);
}
