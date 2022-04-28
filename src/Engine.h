#pragma once

#define ENGINE

#include <memory>
#include "Scene.h"
#include "Core/ECS.h"
#include "Events.h"
// #define DEBUG_ENABLE

namespace ti {
	class Engine {
		ti::ECS::Registry registry;

		bool quit = false;

		double dt;

		SDL_Window* window;
		SDL_GLContext context;
		SDL_Event event;

		const char* title = "Titanium";
		int width = 1280, height = 640, posx = 50, posy = 50;

		std::vector<std::shared_ptr<Layer>> scene_array;

		Events eventdata;

		#ifdef DEBUG_ENABLE
			FrameBuffer *main_fbo = nullptr;
		#endif

	public:
		void ImGuiInit();
		void ImGuiUpdate();
		void ImGuiRender();
		void ImGuiDestroy();

		void CreateContext();
		void Init();
		void Mainloop();
		void EventHandler();
		void Update(double dt);
		void Render();
		void Destroy();

		double& TimeStep();

		template <typename T> void AddScene() {
			auto scene = std::make_shared<T>();

			scene->registry = &registry;
			scene->eventdata = &eventdata;
			scene->width = width;
			scene->height = height;

			scene_array.push_back(std::static_pointer_cast<Layer>(scene));
		}
	};
}