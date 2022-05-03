#include "Sandbox.h"

#include "Core/EngineProperties.h"

#include "Core/Functions.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Sandbox::Init() {
	auto& engine = registry->Store<ti::EngineProperties>();
	registry->Store<ti::Functions>(registry);
	
	registry->Store<ti::Functions>().ImportModel("D:\\C++\\2.5D Engine\\res\\cube.obj");
	registry->Store<ti::Functions>().ImportModel("D:\\C++\\2.5D Engine\\res\\cylinder.obj");

	registry->Store<ti::Functions>().AddLightEntity();

	SDL_ShowCursor(SDL_DISABLE);
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
}
