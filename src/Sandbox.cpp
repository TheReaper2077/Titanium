#include "Sandbox.h"
#include "Core/System/RenderSystem.h"
#include "Core/System/CameraSystem.h"

#include "Core/Functions.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

ti::System::RenderSystem rendersystem;
ti::System::CameraSystem camerasystem;

void Sandbox::Init() {
	auto& engine = registry->Store<ti::EngineProperties>();
	registry->Store<ti::Functions>(registry);

	auto camera = registry->Create();
	registry->Add<ti::Component::Camera>(camera, ti::Projection::PERSPECTIVE, true);
	registry->Add<ti::Component::Transform>(camera);
	registry->Add<ti::Component::Tag>(camera, "cam0", camera);

	rendersystem.registry = registry;
	camerasystem.registry = registry;
	rendersystem.Init();
	camerasystem.Init();
	
	registry->Store<ti::Functions>().ImportModel("D:\\C++\\2.5D Engine\\res\\cube.obj");
	registry->Store<ti::Functions>().ImportModel("D:\\C++\\2.5D Engine\\res\\cylinder.obj");

	auto light = registry->Create();
	registry->Add<ti::Component::Tag>(light, "light", light);
	registry->Add<ti::Component::Transform>(light);
	registry->Add<ti::Component::Light>(light);

	// SDL_ShowCursor(SDL_DISABLE);
}

void Sandbox::Update(double dt) {
	auto& engine = registry->Store<ti::EngineProperties>();

	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glEnable(GL_CULL_FACE);
	glViewport(0, 0, engine.width, engine.height);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	camerasystem.Update(dt);
	rendersystem.Update(dt);
}
