#pragma once

#include <memory>
#include "ECS.h"
#include "Layer.h"

#include <OpenGL.h>
#include <chrono>
#include <iostream>
#include "EngineProperties.h"
#include "Events.h"
#include "ImGuiLayer.h"


namespace ti {
	class Engine {
		ti::ECS::Registry registry;
		std::vector<std::shared_ptr<Layer>> scene_array;
		ImGuiLayer imguilayer;
		
	public:
		void CreateContext() {
			auto& engine = registry.Store<EngineProperties>();

			engine.quit = false;

			OpenGL_CreateContext();
			SDL_Init(SDL_INIT_VIDEO);

			engine.window = SDL_CreateWindow(engine.context_title, engine.context_posx, engine.context_posy, engine.context_width, engine.context_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
			assert(engine.window);
			engine.context = SDL_GL_CreateContext(engine.window);

			gladLoadGLLoader(SDL_GL_GetProcAddress);

			imguilayer.registry = &registry;
			imguilayer.Init();
		}

		void Init() {
			auto& engine = registry.Store<EngineProperties>();

			for (auto& scene: this->scene_array) {
				scene->Init();
			}
		}

		void Mainloop() {
			Init();

			auto& engine = registry.Store<EngineProperties>();

			while (!engine.quit) {
				const auto& start_time = std::chrono::high_resolution_clock::now();

				EventHandler();

				if (engine.dt >= 1/60.0)
					FixedUpdate(engine.dt);

				imguilayer.BeginMain();

				Update(engine.dt);

				imguilayer.EndMain();

				SDL_GL_SwapWindow(engine.window);

				const auto& end_time = std::chrono::high_resolution_clock::now();
				engine.dt = std::chrono::duration<double, std::ratio<1, 60>>(end_time - start_time).count();
			}

			Destroy();
		}

		void EventHandler() {
			auto& events = registry.Store<Events>();
			auto& engine = registry.Store<EngineProperties>();

			events.mouse_scrollup = false;
			events.mouse_scrolldown = false;
			static bool in_window;

			while (SDL_PollEvent(&engine.event)) {
				imguilayer.EventHandler();

				if (engine.event.type == SDL_QUIT) {
					engine.quit = true;
				}
				if (engine.event.type == SDL_MOUSEMOTION) {
					in_window = (engine.event.motion.x >= engine.posx && engine.event.motion.x < engine.width + engine.posx && engine.event.motion.y >= engine.posy && engine.event.motion.y < engine.height + engine.posy);

					if (in_window) {
						events.posx += engine.event.motion.xrel;
						events.posy += engine.event.motion.yrel;
						
						events.relx = engine.event.motion.xrel;
						events.rely = engine.event.motion.yrel;

						events.normalized_mouse.x = ((engine.event.motion.x - engine.posx) / (engine.width * 0.5)) - 1.0;
						events.normalized_mouse.y = 1.0 - ((engine.event.motion.y - engine.posy) / (engine.height * 0.5));
						events.normalized_mouse.z = 0;
					}
				}
				if (engine.event.type == SDL_KEYDOWN && (!engine.debug_mode || in_window)) {
					events.key_pressed.insert(engine.event.key.keysym.scancode);
					if (events.key_chord.size() == 0)
						events.key_chord.push_back(engine.event.key.keysym.scancode);
					else if (events.key_chord.back() != engine.event.key.keysym.scancode)
						events.key_chord.push_back(engine.event.key.keysym.scancode);
				}
				if (engine.event.type == SDL_KEYUP && (!engine.debug_mode || in_window)) {
					events.key_chord.clear();
					events.key_pressed.erase(engine.event.key.keysym.scancode);
				}
				if (engine.event.type == SDL_MOUSEBUTTONDOWN && (!engine.debug_mode || in_window)) {
					events.mouse_pressed.insert(engine.event.button.button);
				}
				if (engine.event.type == SDL_MOUSEBUTTONUP && (!engine.debug_mode || in_window)) {
					events.mouse_pressed.erase(engine.event.button.button);
				}
				if (engine.event.type == SDL_MOUSEWHEEL && (!engine.debug_mode || in_window)) {
					events.mouse_scrollup = (engine.event.wheel.y > 0);
					events.mouse_scrolldown = (engine.event.wheel.y < 0);
				}
				if (engine.debug_mode && !in_window) {
					events.mouse_pressed.clear();
					events.key_pressed.clear();

					events.mouse_scrollup = false;
					events.mouse_scrolldown = false;
				}
			}
		}

		void FixedUpdate(double dt) {

		}

		void Update(double dt) {
			auto& engine = registry.Store<EngineProperties>();

			engine.drawcalls = 0;
			engine.indexcount = 0;
			engine.vertexcount = 0;

			for (auto& scene: scene_array) {
				scene->Update(dt);
			}

			static int count;
			static double time_seconds;

			if (time_seconds >= 60) {
				engine.framerate = count;
				count = 0;
				time_seconds = 0;
			}

			count++;
			time_seconds += dt;
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