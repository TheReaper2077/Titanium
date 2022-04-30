#pragma once

#include "Engine.h"
#include "Components/Components.h"
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_internal.h>
#include <imgui_impl_opengl3.h>

#define DEGREE_TO_RADIANS(x) ((x) * (3.14159265359/180.0))
#define RADIANS_TO_DEGREE(x) ((x) * (180.0/3.14159265359))

namespace ti {
	class ImGuiLayer {
	public:
		ti::ECS::Registry* registry;

		FrameBuffer* main_fbo;

		void Init() {
			auto& engine = registry->Store<EngineProperties>();

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
		}

		void BeginMain() {
			auto& engine = registry->Store<EngineProperties>();

			glViewport(0, 0, engine.context_width, engine.context_height);
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
			ImGui::SetNextWindowSize(ImVec2(engine.context_width, engine.context_height));

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

			ImGui::Begin(engine.title);
			ImGui::PopStyleVar();

			engine.posx = ImGui::GetWindowPos().x;
			engine.posy = ImGui::GetWindowPos().y;
			engine.width = ImGui::GetWindowSize().x;
			engine.height = ImGui::GetWindowSize().y;

			main_fbo->Bind();
		}

		void EndMain() {
			auto& engine = registry->Store<EngineProperties>();

			main_fbo->UnBind();

			ImGui::GetWindowDrawList()->AddImage((void*)main_fbo->id, ImVec2(engine.posx, engine.posy), ImVec2(engine.posx + engine.width, engine.posy + engine.height), ImVec2(0, (float)engine.height/engine.context_height), ImVec2((float)engine.width/engine.context_width, 0));
			ImGui::End();
			
			Update();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 50.0f) {
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];

			ImGui::PushID(label.c_str());

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text("%s", label.c_str());
			ImGui::NextColumn();

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 0 });

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", buttonSize))
				values.x = resetValue;
			ImGui::PopFont();

			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushFont(boldFont);
			if (ImGui::Button("Y", buttonSize))
				values.y = resetValue;
			ImGui::PopFont();

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushFont(boldFont);
			if (ImGui::Button("Z", buttonSize))
				values.z = resetValue;
			ImGui::PopFont();

			ImGui::SameLine();
			ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();

			ImGui::PopStyleVar();

			ImGui::Columns(1);

			ImGui::PopID();
		}

		void Inspector(ti::ECS::Entity entity) {
			using namespace ti::Component;

			static ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;

			ImGui::Begin("Inspector");

			if (registry->Contains<Tag>(entity)) {
				if (ImGui::CollapsingHeader("Tag", flags)) {
					auto& tag = registry->Get<Tag>(entity);

					// ImGui::InputText("Name", tag.name.c_str(), 20*sizeof(char));
				}
			}

			if (registry->Contains<Transform>(entity)) {
				if (ImGui::CollapsingHeader("Transform", flags)) {
					auto& transform = registry->Get<Transform>(entity);

					ImGui::DragFloat3("Position", &transform.position[0], 0.1);
					ImGui::DragFloat3("Rotation", &transform.rotation[0], 0.1);
					ImGui::DragFloat3("Scale", &transform.scale[0], 0.1);
				}
			}

			if (registry->Contains<Camera>(entity)) {
				if (ImGui::CollapsingHeader("Camera", flags)) {
					auto& camera = registry->Get<Camera>(entity);

					ImGui::DragFloat3("Up", &camera.Up[0], 0.1);
					ImGui::DragFloat3("Front", &camera.Front[0], 0.1);
					ImGui::DragFloat3("Center", &camera.Center[0], 0.1);
					ImGui::DragFloat3("Offset", &camera.Offset[0], 0.1);

					ImGui::Checkbox("Enable", &camera.enable);

					ImGui::DragFloat("Sensitivity", &camera.sensitivity, 0.001);
					ImGui::DragFloat("Speed", &camera.speed, 0.001);
					ImGui::DragFloat("ScrollSpeed", &camera.scrollspeed, 0.001);
				}
			}

			if (registry->Contains<MeshFilter>(entity)) {
				if (ImGui::CollapsingHeader("MeshFilter", flags)) {
					auto& meshfilter = registry->Get<MeshFilter>(entity);

					// ImGui::DragFloat3("Position", &meshfilter.position[0], 0.1, 0.0, 0.0, "%.2f", 0.1);
					// ImGui::DragFloat3("Rotaion", &meshfilter.rotation[0], 0.1, 0.0, 0.0, "%.2f", 0.1);
					// ImGui::DragFloat3("Scale", &meshfilter.scale[0], 0.1, 0.0, 0.0, "%.2f", 0.1);
				}
			}

			ImGui::End();
		}

		ti::ECS::Entity Heirarchy() {
			using namespace ti::Component;

			ImGui::Begin("Scene Hierarchy");

			static ti::ECS::Entity selected_entity;

			for (auto& entity: registry->View<Tag>()) {
				auto& tag = registry->Get<Tag>(entity);

				ImGui::TreeNodeEx(tag.name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);

				if (ImGui::IsItemClicked()) {
					selected_entity = entity;
				}
			}

			ImGui::End();

			return selected_entity;
		}

		void Status() {
			ImGui::Begin("Status");

			auto& engine = registry->Store<EngineProperties>();

			ImGui::Text("%s: %i", "DrawCalls", engine.drawcalls);
			ImGui::Text("%s: %i", "Framerate", engine.framerate);
			ImGui::Text("%s: %i", "Vertexcount", engine.vertexcount);
			ImGui::Text("%s: %i", "Indexcount", engine.indexcount);

			ImGui::End();
		}

		void Update() {
			using namespace ti::Component;

			auto entity = Heirarchy();
			Inspector(entity);
			Status();
		}

		void EventHandler() {
			auto& engine = registry->Store<EngineProperties>();

			ImGui_ImplSDL2_ProcessEvent(&engine.event);
		}
	};
}
