#pragma once

#include "Engine.h"
#include "Components/Components.h"
#include "MaterialRegistry.h"
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>
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

					ImGui::Checkbox("Enable", &camera.enable);

					ImGui::DragFloat("Sensitivity", &camera.sensitivity, 0.001);
					ImGui::DragFloat("Speed", &camera.speed, 0.001);
					ImGui::DragFloat("ScrollSpeed", &camera.scrollspeed, 0.001);

					static std::string total_modes[] = { "FPS", "TPS", "Editor" };
					if (ImGui::BeginCombo("Type", total_modes[camera.type].c_str(), ImGuiComboFlags_NoArrowButton)) {
						for (int i = 0; i < LightMode_COUNT; i++) {
							if (total_modes[i] != total_modes[camera.type])
								if (ImGui::Selectable(total_modes[i].c_str(), true))
									camera.type = (CameraType)i;
						}
						ImGui::EndCombo();
					}
				}
			}

			if (registry->Contains<Light>(entity)) {
				if (ImGui::CollapsingHeader("Light", flags)) {
					auto& light = registry->Get<Light>(entity);

					static std::string total_modes[] = { "Point", "Flash", "Spot", "Area", "Directional" };
					if (ImGui::BeginCombo("mode", total_modes[light.mode].c_str(), ImGuiComboFlags_NoArrowButton)) {
						for (int i = 0; i < LightMode_COUNT; i++) {
							if (total_modes[i] != total_modes[light.mode])
								if (ImGui::Selectable(total_modes[i].c_str(), true))
									light.mode = (LightMode)i;
						}
						ImGui::EndCombo();
					}

					if (light.mode == Directional) {
						ImGui::DragFloat3("Ambient", &light.ambient[0], 0.001, 0.0, 1.0, "%.3f");
						ImGui::DragFloat3("Diffuse", &light.diffuse[0], 0.001, 0.0, 1.0, "%.3f");
						ImGui::DragFloat3("Specular", &light.specular[0], 0.001, 0.0, 1.0, "%.3f");
					}
					if (light.mode == Point) {
						ImGui::DragFloat3("Ambient", &light.ambient[0], 0.001, 0.0, 1.0, "%.3f");
						ImGui::DragFloat3("Diffuse", &light.diffuse[0], 0.001, 0.0, 1.0, "%.3f");
						ImGui::DragFloat3("Specular", &light.specular[0], 0.001, 0.0, 1.0, "%.3f");

						ImGui::DragFloat("Constant", &light.constant, 0.001, 0.0, 1.0, "%.3f");
						ImGui::DragFloat("Linear", &light.linear, 0.001, 0.0, 1.0, "%.3f");
						ImGui::DragFloat("Quadratic", &light.quadratic, 0.001, 0.0, 2.0, "%.3f");
					}
				}
			}

			if (registry->Contains<MeshRenderer>(entity)) {
				if (ImGui::CollapsingHeader("Material", flags)) {
					auto& meshrenderer = registry->Get<MeshRenderer>(entity);

					if (ImGui::BeginCombo("Material", meshrenderer.material.c_str(), ImGuiComboFlags_NoArrowButton)) {
						for (auto& pair: registry->Store<ti::MaterialRegistry>().registry) {
							if (meshrenderer.material != pair.first)
								if (ImGui::Selectable(pair.first.c_str(), true))
									meshrenderer.material = pair.first;
						}
						ImGui::EndCombo();
					}
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
			
			if (ImGui::Button("New Entity")) {
				// if (ImGui::Popup)
			}

			ImGui::End();

			return selected_entity;
		}

		void MaterialRegistry() {
			using namespace ti::Component;

			ImGui::Begin("Material Registry");

			for (auto& pair: registry->Store<ti::MaterialRegistry>().registry) {
				auto& name = pair.first;
				auto& material = pair.second;

				if (ImGui::TreeNode(name.c_str())) {
					ImGui::DragFloat3("Ambient", &material.ambient[0], 0.001, 0.0, 1.0, "%.3f");
					ImGui::DragFloat3("Diffuse", &material.diffuse[0], 0.001, 0.0, 1.0, "%.3f");
					ImGui::DragFloat3("Specular", &material.specular[0], 0.001, 0.0, 1.0, "%.3f");
					ImGui::DragFloat("Shininess", &material.shininess, 0.1);

					ImGui::TreePop();
				}
			}

			ImGui::End();
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
			MaterialRegistry();
		}

		void EventHandler() {
			auto& engine = registry->Store<EngineProperties>();

			ImGui_ImplSDL2_ProcessEvent(&engine.event);
		}
	};
}
