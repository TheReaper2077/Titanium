#pragma once

#include "Transform.h"
#include "Renderer.h"
#include <entt/entt.hpp>
#include <memory>

namespace ti {
	struct Engine {
		entt::registry registry;

		bool quit = false;

		double dt;

		SDL_Window* window;
		SDL_GLContext context;
		SDL_Event event;

		const char* title = "Titanium";
		int width = 1280, height = 640, posx = 50, posy = 50;

		Renderer renderer;

		std::vector<SDL_Scancode> key_chord;
	};

	void ImGuiInit();
	void ImGuiUpdate();
	void ImGuiRender();
	void ImGuiDestroy();

	void Init();
	void Mainloop();
	void EventHandler();
	void Update(double dt);
	void Render();
	void Destroy();

	double& TimeStep();

	Engine* GetEngine();
	SDL_Event* GetEvent();
}