#include "Sandbox.h"

#include "Core/EngineProperties.h"

#include "Core/Functions.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Core/Serializer.h"

void Sandbox::Init() {
	registry->Store<ti::Functions>(registry);
	registry->Store<ti::Serializer>(registry);

	registry->Store<ti::Functions>().ImportModel("D:\\C++\\2.5D Engine\\res\\cube.obj");
	registry->Store<ti::Functions>().ImportModel("D:\\C++\\2.5D Engine\\res\\cylinder.obj");

	SDL_ShowCursor(SDL_DISABLE);

	registry->Store<ti::Serializer>().DeSerialize();
}

void Sandbox::Update(double dt) {
	auto& engine = registry->Store<ti::EngineProperties>();
	auto& events = registry->Store<ti::Events>();

	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glEnable(GL_CULL_FACE);
	glViewport(0, 0, engine.width, engine.height);
	glClearColor(0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (events.key_pressed.contains(SDL_SCANCODE_S) && events.key_pressed.contains(SDL_SCANCODE_LCTRL)) {
		registry->Store<ti::Serializer>().Serialize();
	}
}
