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

		#ifdef TI_DEBUG

		FrameBuffer* main_fbo = nullptr;

		#endif
		
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

			#ifdef TI_DEBUG

			if (main_fbo == nullptr)
				main_fbo = FrameBuffer_Create(engine.width, engine.height);

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO &io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
			io.ConfigDockingWithShift = true;

			ImGui_ImplSDL2_InitForOpenGL(engine.window, (void*)engine.context);
			ImGui_ImplOpenGL3_Init("#version 400");

			ImGui::StyleColorsDark();

			#endif
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

				#ifdef TI_DEBUG

				ImGui_ImplSDL2_ProcessEvent(&engine.event);

				#endif

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
			auto& engine = registry.Store<EngineProperties>();

			#ifdef TI_DEBUG

			glViewport(0, 0, engine.width, engine.height);
			glClearColor(0.2, 0.2, 0.2, 0.2);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();

			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(engine.width, engine.height));

			static bool p_open;
			ImGui::Begin("DockSpace", &p_open, window_flags);
				ImGui::PopStyleVar();
				ImGui::PopStyleVar(2);

				if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable) {
					ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
					ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
				}
			ImGui::End();

			static bool show_demo = true;
			ImGui::ShowDemoWindow(&show_demo);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::Begin("Game");
			ImGui::PopStyleVar();

			const auto& size = ImGui::GetWindowSize();
			const auto& pos = ImGui::GetWindowPos();

			main_fbo->Bind();

			#endif

			for (auto& scene: scene_array) {
				scene->Update(dt);
			}

			#ifdef TI_DEBUG

			main_fbo->UnBind();

			ImGui::GetWindowDrawList()->AddImage((void*)main_fbo->id, pos, ImVec2(pos.x + size.x, pos.y + size.y), ImVec2(0, 1), ImVec2(1, 0));
			
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			#endif
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