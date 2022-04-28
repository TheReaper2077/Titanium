#pragma once

#include <memory>
#include "ECS.h"
#include "Layer.h"

#include <OpenGL.h>
#include <chrono>
#include "EngineProperties.h"
#include "Events.h"

namespace ti {
	class Engine {
		ti::ECS::Registry registry;
		std::vector<std::shared_ptr<Layer>> scene_array;

	public:
		void CreateContext() {
			auto& engine = registry.Store<EngineProperties>();
			
			engine.quit = false;
	
			OpenGL_CreateContext();
			SDL_Init(SDL_INIT_VIDEO);

			engine.window = SDL_CreateWindow(engine.title, engine.posx, engine.posy, engine.width, engine.height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
			assert(engine.window);
			engine.context = SDL_GL_CreateContext(engine.window);

			gladLoadGLLoader(SDL_GL_GetProcAddress);
		}

		void Init() {
			for (auto& scene: this->scene_array) {
				scene->Init();
			}

			SDL_SetRelativeMouseMode(SDL_TRUE);
		}

		void Mainloop() {
			Init();

			auto& engine = registry.Store<EngineProperties>();

			while (!engine.quit) {
				const auto& start_time = std::chrono::high_resolution_clock::now();

				EventHandler();
				Update(engine.dt);

				SDL_GL_SwapWindow(engine.window);

				const auto& end_time = std::chrono::high_resolution_clock::now();
				engine.dt = std::chrono::duration<double, std::ratio<1, 60>>(end_time - start_time).count();
			}

			Destroy();
		}

		void EventHandler() {
			auto& eventdata = registry.Store<Events>();
			auto& engine = registry.Store<EngineProperties>();

			while (SDL_PollEvent(&engine.event)) {
				if (engine.event.type == SDL_QUIT) {
					engine.quit = true;
				}
				if (engine.event.type == SDL_KEYDOWN) {
					eventdata.key_pressed.insert(engine.event.key.keysym.scancode);
					if (eventdata.key_chord.size() == 0)
						eventdata.key_chord.push_back(engine.event.key.keysym.scancode);
					else if (eventdata.key_chord.back() != engine.event.key.keysym.scancode)
						eventdata.key_chord.push_back(engine.event.key.keysym.scancode);
				}
				if (engine.event.type == SDL_KEYUP) {
					eventdata.key_chord.clear();
					eventdata.key_pressed.erase(engine.event.key.keysym.scancode);
				}
				if (engine.event.type == SDL_MOUSEBUTTONDOWN) {
					eventdata.mouse_pressed.insert(engine.event.button.button);
				}
				if (engine.event.type == SDL_MOUSEBUTTONUP) {
					eventdata.mouse_pressed.erase(engine.event.button.button);
				}
				if (engine.event.type == SDL_MOUSEMOTION) {
					// if (SDL_GetRelativeMouseMode()) {
						eventdata.posx += engine.event.motion.xrel;
						eventdata.posy += engine.event.motion.yrel;
					// } else {
					// 	eventdata.mouseposx = engine.event.motion.x;
					// 	eventdata.mouseposy = engine.event.motion.y;
					// }
					eventdata.relx = engine.event.motion.xrel;
					eventdata.rely = engine.event.motion.yrel;
				}
			}
		}

		void Update(double dt) {
			for (auto& scene: scene_array) {
				scene->Update(dt);
			}
		}

		void Destroy() {
			
		}

		template <typename T> void AddScene() {
			auto scene = std::make_shared<T>();

			scene->registry = &registry;

			scene_array.push_back(std::static_pointer_cast<Layer>(scene));
		}
	};
}