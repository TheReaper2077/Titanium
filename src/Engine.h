#pragma once

#include "Transform.h"
#include "Renderer.h"
#include <entt/entt.hpp>
#include <memory>
#include "Scene.h"

// #define DEBUG_ENABLE

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
		std::vector<Scene> scene_array;

		#ifdef DEBUG_ENABLE
			FrameBuffer *main_fbo = nullptr;
		#endif
	};

	void ImGuiInit();
	void ImGuiUpdate();
	void ImGuiRender();
	void ImGuiDestroy();

	template <typename T> void AddScene() {
		auto scene = std::make_shared<T>();
	}

	void Init();
	void Mainloop();
	void EventHandler();
	void Update(double dt);
	void Render();
	void Destroy();

	double& TimeStep();

	Engine* GetEngine();
}