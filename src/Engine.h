#pragma once

#include "Transform.h"
#include "Renderer.h"
#include <entt/entt.hpp>
#include <memory>

struct Engine {
	entt::registry registry;

	bool quit;

	double dt;

	SDL_Window* window;
	SDL_GLContext context;

	const char* title;
	int width, height, posx, posy;

	Renderer renderer;
};

void ImGuiInit();
void ImGuiUpdate();
void ImGuiRender();
void ImGuiDestroy();

void Init();
void Mainloop();
void Update(double dt);
void Render();
void Destroy();

double& TimeStep();