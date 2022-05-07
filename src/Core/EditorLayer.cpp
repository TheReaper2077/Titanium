#include "EditorLayer.h"

#include "MeshRegistry.h"
#include <deque>
#include "Engine.h"
#include "Components/Components.h"
#include "MaterialRegistry.h"
#include "Functions.h"
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

void ti::ImGuiLayer::Init() {
	auto& engine = registry->Store<EngineProperties>();

	if (main_fbo == nullptr)
		main_fbo = FrameBuffer_Create(engine.width, engine.height);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
	io.ConfigDockingWithShift = true;

	ImGui_ImplSDL2_InitForOpenGL(engine.window, (void*)engine.context);
	ImGui_ImplOpenGL3_Init("#version 400");

	ImGui::StyleColorsDark();
	auto& style = ImGui::GetStyle();
	style.TabRounding = 0;
	style.WindowRounding = 0;
	style.ScrollbarRounding = 0;
	style.FrameBorderSize = 1;
}

void ti::ImGuiLayer::BeginMain() {
	auto& engine = registry->Store<EngineProperties>();

	glViewport(0, 0, engine.context_width, engine.context_height);
	glClearColor(0.2, 0.2, 0.2, 0.2);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

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

void ti::ImGuiLayer::EndMain() {
	auto& engine = registry->Store<EngineProperties>();

	main_fbo->UnBind();

	ImGui::GetWindowDrawList()->AddImage((void*)main_fbo->id, ImVec2(engine.posx, engine.posy), ImVec2(engine.posx + engine.width, engine.posy + engine.height), ImVec2(0, (float)engine.height / engine.context_height), ImVec2((float)engine.width / engine.context_width, 0));
	ImGui::End();

	Update();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ti::ImGuiLayer::Inspector(ti::ECS::Entity& entity) {
	using namespace ti::Component;

	static ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
	static ECS::Entity last_entity;

	if (!entity) return;

	static bool camera_open;
	static bool light_open;
	static bool meshfilter_open;
	static bool spriterenderer_open;
	static bool meshrenderer_open;

	auto& funnctions = registry->Store<Functions>();

	if (entity != last_entity) {
		last_entity = entity;

		camera_open = true;
		light_open = true;
		meshfilter_open = true;
		spriterenderer_open = true;
		meshrenderer_open = true;
	}

	ImGui::Begin("Inspector");

	if (registry->Contains<Tag>(entity)) {
		if (ImGui::CollapsingHeader("Tag", flags)) {
			auto& tag = registry->Get<Tag>(entity);

			ImGui::InputText("name", &tag.name);
		}
	}

	if (registry->Contains<Transform>(entity)) {
		if (ImGui::CollapsingHeader("Transform", flags)) {
			auto& transform = registry->Get<Transform>(entity);

			ImGui::DragFloat3("Position", &transform.position[0], 0.1);
			ImGui::DragFloat3("Rotation", &transform.rotation[0], 0.1);
			ImGui::DragFloat3("Scale", &transform.scale[0], 0.1);

			auto& editorcamera = registry->Get<Camera>(registry->Store<ti::Functions>().editor_camera);
		}
	}

	if (registry->Contains<Camera>(entity)) {
		if (ImGui::CollapsingHeader("Camera", &camera_open, flags)) {
			auto& camera = registry->Get<Camera>(entity);

			ImGui::Checkbox("Enable", &camera.enable);

			ImGui::Spacing();

			ImGui::DragFloat("Sensitivity", &camera.sensitivity, 0.001);
			ImGui::DragFloat("Speed", &camera.speed, 0.001);
			ImGui::DragFloat("ScrollSpeed", &camera.scrollspeed, 0.001);

			ImGui::Spacing();

			static const char* total_modes[] = { "FPS", "TPS", "Editor" };
			if (ImGui::BeginCombo("Type", total_modes[camera.type], ImGuiComboFlags_NoArrowButton)) {
				for (int i = 0; i < CameraType_COUNT; i++) {
					if (total_modes[i] != total_modes[camera.type])
						if (ImGui::Selectable(total_modes[i], true))
							camera.type = (CameraType)i;
				}
				ImGui::EndCombo();
			}
		} else if (!camera_open) {
			registry->Remove<Camera>(entity);
		}
	}

	if (registry->Contains<Light>(entity)) {
		if (ImGui::CollapsingHeader("Light", &light_open, flags)) {

			auto& light = registry->Get<Light>(entity);

			static std::string total_modes[] = { "Point", "Spot", "Area", "Directional" };
			if (ImGui::BeginCombo("mode", total_modes[light.mode].c_str(), ImGuiComboFlags_NoArrowButton)) {
				for (int i = 0; i < LightMode_COUNT; i++) {
					if (total_modes[i] != total_modes[light.mode])
						if (ImGui::Selectable(total_modes[i].c_str(), true))
							light.mode = (LightMode)i;
				}
				ImGui::EndCombo();
			}

			ImGui::Spacing();

			ImGui::Checkbox("Active", &light.active);

			ImGui::Spacing();

			ImGui::ColorPicker3("Color", &light.color[0]);

			// if (light.mode == Directional) {
			// 	ImGui::DragFloat3("Ambient", &light.ambient[0], 0.001, 0.0, 1.0, "%.3f");
			// 	ImGui::DragFloat3("Diffuse", &light.diffuse[0], 0.001, 0.0, 1.0, "%.3f");
			// 	ImGui::DragFloat3("Specular", &light.specular[0], 0.001, 0.0, 1.0, "%.3f");
			// }
			// if (light.mode == Point) {
			// 	ImGui::DragFloat3("Ambient", &light.ambient[0], 0.001, 0.0, 1.0, "%.3f");
			// 	ImGui::DragFloat3("Diffuse", &light.diffuse[0], 0.001, 0.0, 1.0, "%.3f");
			// 	ImGui::DragFloat3("Specular", &light.specular[0], 0.001, 0.0, 1.0, "%.3f");

			// 	ImGui::DragFloat("Constant", &light.constant, 0.001, 0.0, 1.0, "%.3f");
			// 	ImGui::DragFloat("Linear", &light.linear, 0.001, 0.0, 1.0, "%.3f");
			// 	ImGui::DragFloat("Quadratic", &light.quadratic, 0.001, 0.0, 2.0, "%.3f");
			// }
		} else if (!light_open) {
			registry->Remove<Light>(entity);
		}
	}

	if (registry->Contains<MeshFilter>(entity)) {
		if (ImGui::CollapsingHeader("MeshFilter", &meshfilter_open, flags)) {
			auto& meshfilter = registry->Get<MeshFilter>(entity);

			if (ImGui::BeginCombo("Mesh", meshfilter.mesh.c_str(), ImGuiComboFlags_NoArrowButton)) {
				for (auto& pair : registry->Store<ti::MeshRegistry>().registry) {
					if (ImGui::Selectable(pair.first.c_str()))
						meshfilter.mesh = pair.first;
				}
				ImGui::EndCombo();
			}
		} else if (!meshfilter_open) {
			registry->Remove<MeshFilter>(entity);
		}
	}

	if (registry->Contains<SpriteRenderer>(entity)) {
		if (ImGui::CollapsingHeader("SpriteRenderer", &spriterenderer_open, flags)) {
			auto& spriterenderer = registry->Get<SpriteRenderer>(entity);

			ImGui::Spacing();

			ImGui::Checkbox("Visible", &spriterenderer.visible);

			ImGui::Spacing();

			ImGui::ColorPicker4("Color", &spriterenderer.color[0]);
		} else if (!spriterenderer_open) {
			registry->Remove<SpriteRenderer>(entity);
		}
	}

	if (registry->Contains<MeshRenderer>(entity)) {
		if (ImGui::CollapsingHeader("MeshRenderer", &meshrenderer_open, flags)) {
			auto& meshrenderer = registry->Get<MeshRenderer>(entity);

			ImGui::Spacing();

			ImGui::Checkbox("Visible", &meshrenderer.visible);

			ImGui::Spacing();

			if (ImGui::BeginCombo("Material", meshrenderer.material.c_str(), ImGuiComboFlags_NoArrowButton)) {
				for (auto& pair : registry->Store<ti::MaterialRegistry>().registry) {
					if (ImGui::Selectable(pair.second.name.c_str()))
						meshrenderer.material = pair.second.name;
				}
				ImGui::EndCombo();
			}

			ImGui::Spacing();

			static bool check;

			check = (meshrenderer.flags & POSITION_ATTRIB_BIT);
			ImGui::Checkbox("POSITION", &check);
			check = (meshrenderer.flags & NORMAL_ATTRIB_BIT);
			ImGui::Checkbox("NORMAL", &check);
			check = (meshrenderer.flags & COLOR_ATTRIB_BIT);
			ImGui::Checkbox("COLOR", &check);
			check = (meshrenderer.flags & UV0_ATTRIB_BIT);
			ImGui::Checkbox("UV0", &check);
			check = (meshrenderer.flags & UV1_ATTRIB_BIT);
			ImGui::Checkbox("UV1", &check);
			check = (meshrenderer.flags & UV2_ATTRIB_BIT);
			ImGui::Checkbox("UV2", &check);
			check = (meshrenderer.flags & UV3_ATTRIB_BIT);
			ImGui::Checkbox("UV3", &check);
			check = (meshrenderer.flags & UV4_ATTRIB_BIT);
			ImGui::Checkbox("UV4", &check);
			check = (meshrenderer.flags & UV5_ATTRIB_BIT);
			ImGui::Checkbox("UV5", &check);
			check = (meshrenderer.flags & UV6_ATTRIB_BIT);
			ImGui::Checkbox("UV6", &check);
			check = (meshrenderer.flags & UV7_ATTRIB_BIT);
			ImGui::Checkbox("UV7", &check);
		} else if (!meshrenderer_open) {
			registry->Remove<MeshRenderer>(entity);
		}
	}

	ImGui::Spacing();

	if (ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		ImGui::OpenPopup("New Component");

	if (ImGui::BeginPopup("New Component")) {
		if (ImGui::Selectable("MeshFilter") && !registry->Contains<MeshFilter>(entity)) {
			registry->Add<MeshFilter>(entity);
			meshfilter_open = true;
		}
		if (ImGui::Selectable("MeshRenderer") && !registry->Contains<MeshRenderer>(entity)) {
			registry->Add<MeshRenderer>(entity);
			meshrenderer_open = true;
		}
		if (ImGui::Selectable("SpriteRenderer") && !registry->Contains<SpriteRenderer>(entity)) {
			registry->Add<SpriteRenderer>(entity);
			spriterenderer_open = true;
		}
		if (ImGui::Selectable("Camera") && !registry->Contains<Camera>(entity)) {
			registry->Add<Camera>(entity);
			camera_open = true;
		}
		if (ImGui::Selectable("Light") && !registry->Contains<Light>(entity)) {
			registry->Add<Light>(entity);
			light_open = true;
		}
		ImGui::EndPopup();
	}

	ImGui::End();
}

ti::ECS::Entity ti::ImGuiLayer::Heirarchy() {
	using namespace ti::Component;

	auto& events = registry->Store<Events>();

	ImGui::Begin("Scene Hierarchy");

	static ti::ECS::Entity selected_entity;
	static bool hovered;

	for (auto& entity : registry->View<Tag>()) {
		auto& tag = registry->Get<Tag>(entity);

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (selected_entity == entity)
			flags |= ImGuiTreeNodeFlags_Selected;

		ImGui::TreeNodeEx(tag.name.c_str(), flags);

		if (selected_entity == entity)
			hovered = ImGui::IsItemFocused();

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			ImGui::OpenPopup("Properties");
		}
		
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || !selected_entity) {
			selected_entity = entity;
		}
	}

	if (ImGui::BeginPopup("Properties")) {
		if (ImGui::Selectable("Delete")) {
			registry->Destroy(selected_entity);
			selected_entity = 0;
		}
		if (ImGui::Selectable("Duplicate")) {
			registry->Store<Functions>().DuplicateEntity(selected_entity);
		}

		ImGui::EndPopup();
	}

	if (hovered && events.key_pressed.contains(SDL_SCANCODE_LCTRL) && events.key_pressed.contains(SDL_SCANCODE_D))
		registry->Store<Functions>().DuplicateEntity(selected_entity);
	
	ImGui::Spacing();

	if (ImGui::Button("Add Enity", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		ImGui::OpenPopup("New Entity");

	if (ImGui::BeginPopup("New Entity")) {
		auto& functions = registry->Store<ti::Functions>();

		if (ImGui::Selectable("Empty Entity")) {
			selected_entity = functions.AddEmptyEntity();
		}
		if (ImGui::Selectable("Mesh Entity")) {
			selected_entity = functions.AddMeshEntity();
		}
		if (ImGui::Selectable("Sprite Entity")) {
			selected_entity = functions.AddSpriteEntity();
		}
		if (ImGui::Selectable("Camera Entity")) {
			selected_entity = functions.AddCameraEntity();
		}
		if (ImGui::Selectable("Light Entity")) {
			selected_entity = functions.AddLightEntity();
		}

		ImGui::EndPopup();
	}

	ImGui::End();

	return selected_entity;
}

void ti::ImGuiLayer::MaterialRegistry() {
	using namespace ti::Component;

	ImGui::Begin("Material Registry");

	for (auto& pair : registry->Store<ti::MaterialRegistry>().registry) {
		auto& name = pair.first;
		auto& material = pair.second;

		if (ImGui::TreeNode(name.c_str())) {
			ImGui::DragFloat3("Ambient", &material.ambient[0], 0.001, 0.0, 1.0, "%.3f");
			ImGui::DragFloat3("Diffuse", &material.diffuse[0], 0.001, 0.0, 1.0, "%.3f");
			ImGui::DragFloat3("Specular", &material.specular[0], 0.001, 0.0, 1.0, "%.3f");
			ImGui::DragFloat3("Albedo", &material.albedo[0], 0.001, 0.0, 1.0, "%.3f");
			ImGui::DragFloat("Roughness", &material.roughness, 0.001, 0.0, 1.0, "%.3f");
			ImGui::DragFloat("Metallic", &material.metallic, 0.001, 0.0, 1.0, "%.3f");
			ImGui::DragFloat("Shininess", &material.shininess, 0.1);

			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void ti::ImGuiLayer::MeshRegistry() {
	using namespace ti::Component;

	ImGui::Begin("Mesh Registry");

	for (auto& pair : registry->Store<ti::MeshRegistry>().registry) {
		auto& name = pair.first;
		auto& mesh = pair.second;

		if (ImGui::TreeNode(name.c_str())) {


			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void ti::ImGuiLayer::Status() {
	ImGui::Begin("Status");

	auto& engine = registry->Store<EngineProperties>();

	ImGui::Text("%s: %i", "DrawCalls", engine.drawcalls);
	ImGui::Text("%s: %i", "Framerate", engine.framerate);
	ImGui::Text("%s: %i", "Vertexcount", engine.vertexcount);
	ImGui::Text("%s: %i", "Indexcount", engine.indexcount);

	ImGui::End();
}

void ti::ImGuiLayer::Update() {
	using namespace ti::Component;
	auto& functions = registry->Store<Functions>();
	auto& events = registry->Store<Events>();

	functions.runtime_creation = false;

	auto entity = Heirarchy();
	Inspector(entity);
	Status();
	MaterialRegistry();
	MeshRegistry();

	// if (events.key_pressed.contains(SDL_SCANCODE_S) && events.key_pressed.contains(SDL_SCANCODE_LCTRL)) {
	// 	// registry->Store<ti::Serializer>().Serialize();
	// }

	functions.runtime_creation = true;
}

void ti::ImGuiLayer::EventHandler() {
	auto& engine = registry->Store<EngineProperties>();

	ImGui_ImplSDL2_ProcessEvent(&engine.event);
}

