#pragma once

#include "Engine.h"
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_internal.h>
#include <imgui_impl_opengl3.h>

namespace ti {
	class ImGuiLayer {
	public:
		ti::ECS::Registry* registry;

		FrameBuffer* main_fbo;

		ImVec2 size;
		ImVec2 pos;

		void Init() {
			auto& engine = registry->Store<EngineProperties>();

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO &io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
			io.ConfigDockingWithShift = true;
			
			ImGui_ImplSDL2_InitForOpenGL(engine.window, (void*)engine.context);
			ImGui_ImplOpenGL3_Init("#version 400");
			
			ImGui::StyleColorsDark();

			main_fbo = FrameBuffer_Create(engine.width, engine.height);
		}

		void BeginMain() {
			auto& engine = registry->Store<EngineProperties>();

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

			size = ImGui::GetWindowSize();
			pos = ImGui::GetWindowPos();

			main_fbo->Bind();
		}

		void EndMain() {
			main_fbo->UnBind();

			ImGui::GetWindowDrawList()->AddImage((void*)main_fbo->id, pos, ImVec2(pos.x + size.x, pos.y + size.y), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void Update(double dt) {
			
		}

		void EventHandler() {
			auto& engine = registry->Store<EngineProperties>();

			ImGui_ImplSDL2_ProcessEvent(&engine.event);
		}
	};
}
