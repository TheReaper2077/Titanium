#include "Engine.h"

void ti::Engine::CreateContext() {
	this->quit = false;
	
	OpenGL_CreateContext();
	SDL_Init(SDL_INIT_VIDEO);

	this->window = SDL_CreateWindow(this->title, this->posx, this->posy, this->width, this->height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	assert(this->window);
	this->context = SDL_GL_CreateContext(this->window);

	gladLoadGLLoader(SDL_GL_GetProcAddress);
}

void ti::Engine::Destroy() {
	
}

void ti::Engine::Mainloop() {
	Init();
	while (!this->quit) {
		const auto& start_time = std::chrono::high_resolution_clock::now();

		EventHandler();
		Update(this->dt);
		Render();

		const auto& end_time = std::chrono::high_resolution_clock::now();
		this->dt = std::chrono::duration<double, std::ratio<1, 60>>(end_time - start_time).count();
	}

	Destroy();
}

void ti::Engine::Init() {	
	auto& registry = this->registry;
	auto& renderer = this->renderer;

	renderer.Init(this->window);

	for (auto& scene: this->scene_array) {
		scene->Init();
	}

	#ifdef DEBUG_ENABLE
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
		io.ConfigDockingWithShift = true;
		
		ImGui_ImplSDL2_InitForOpenGL(this->window, (void*)this->context);
		ImGui_ImplOpenGL3_Init("#version 400");
		
		ImGui::StyleColorsDark();

		this->main_fbo = FrameBuffer_Create(this->width, this->height);
	#endif

	
}

void ti::Engine::EventHandler() {
	while (SDL_PollEvent(&this->event)) {

		#ifdef DEBUG_ENABLE
			ImGui_ImplSDL2_ProcessEvent(&this->event);
		#endif

		if (this->event.type == SDL_QUIT) {
			this->quit = true;
		}
		if (this->event.type == SDL_KEYDOWN) {
			if (this->key_chord.size() == 0)
				this->key_chord.push_back(this->event.key.keysym.scancode);
			else if (this->key_chord.back() != this->event.key.keysym.scancode)
				this->key_chord.push_back(this->event.key.keysym.scancode);
		}
		if (this->event.type == SDL_KEYUP) {
			this->key_chord.clear();
		}
	}
}

void ti::Engine::Update(double dt) {
	auto& registry = this->registry;

	for (auto& scene: this->scene_array) {
		scene->Update(dt);
	}
}

void ti::Engine::Render() {
	auto& renderer = this->renderer;
	auto& registry = this->registry;

	#ifdef DEBUG_ENABLE
		glViewport(0, 0, this->width, this->height);
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
		ImGui::SetNextWindowSize(ImVec2(this->width, this->height));
		
		static bool p_open;
		ImGui::Begin("DockSpace", &p_open, window_flags);
			ImGui::PopStyleVar();
			ImGui::PopStyleVar(2);

			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable) {
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}
		ImGui::End();
		
		static bool b = true;
		ImGui::ShowDemoWindow(&b);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Game");
			ImGui::PopStyleVar();

			const auto& size = ImGui::GetWindowSize();
			const auto& pos = ImGui::GetWindowPos();

			this->main_fbo->Bind();
			{
	#endif
	
	for (auto& scene: this->scene_array) {
		scene->Render();
	}

	this->renderer.RenderPreset();

	#ifdef DEBUG_ENABLE
			}
			this->main_fbo->UnBind();
			ImGui::GetWindowDrawList()->AddImage((void*)this->main_fbo->id, pos, ImVec2(pos.x + size.x, pos.y + size.y), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

		ImGuiRender();

		ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	#endif

	SDL_GL_SwapWindow(this->window);
}

// double& ti::TimeStep() {
// 	return this->dt;
// }

// ti::Engine* ti::GetEngine() {
// 	return engine.get();
// }