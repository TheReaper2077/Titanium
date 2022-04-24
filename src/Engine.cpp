#include "Engine.h"

static std::unique_ptr<ti::Engine> engine;

void CreateContext() {
	engine = std::make_unique<ti::Engine>();

	engine->quit = false;
	
	OpenGL_CreateContext();
	SDL_Init(SDL_INIT_VIDEO);

	engine->window = SDL_CreateWindow(engine->title, engine->posx, engine->posy, engine->width, engine->height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	assert(engine->window);
	engine->context = SDL_GL_CreateContext(engine->window);

	gladLoadGLLoader(SDL_GL_GetProcAddress);
}

void ti::Destroy() {
	
}

void ti::Mainloop() {
	CreateContext();
	Init();

	while (!engine->quit) {
		const auto& start_time = std::chrono::high_resolution_clock::now();

		EventHandler();
		Update(engine->dt);
		Render();

		const auto& end_time = std::chrono::high_resolution_clock::now();
		engine->dt = std::chrono::duration<double, std::ratio<1, 60>>(end_time - start_time).count();
	}

	Destroy();
}

void ti::Init() {
	auto& registry = engine->registry;
	auto& renderer = engine->renderer;

	renderer.Init(engine->window);

	#ifdef DEBUG_ENABLE
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
		io.ConfigDockingWithShift = true;
		
		ImGui_ImplSDL2_InitForOpenGL(engine->window, (void*)engine->context);
		ImGui_ImplOpenGL3_Init("#version 400");
		
		ImGui::StyleColorsDark();

		engine->main_fbo = FrameBuffer_Create(engine->width, engine->height);
	#endif
}

void ti::EventHandler() {
	while (SDL_PollEvent(&engine->event)) {

		#ifdef DEBUG_ENABLE
			ImGui_ImplSDL2_ProcessEvent(&engine->event);
		#endif

		if (engine->event.type == SDL_QUIT) {
			engine->quit = true;
		}
		if (engine->event.type == SDL_KEYDOWN) {
			if (engine->key_chord.size() == 0)
				engine->key_chord.push_back(engine->event.key.keysym.scancode);
			else if (engine->key_chord.back() != engine->event.key.keysym.scancode)
				engine->key_chord.push_back(engine->event.key.keysym.scancode);
		}
		if (engine->event.type == SDL_KEYUP) {
			engine->key_chord.clear();
		}
	}
}

void ti::Update(double dt) {
	auto& registry = engine->registry;
}

void ti::Render() {
	auto& renderer = engine->renderer;
	auto& registry = engine->registry;

	#ifdef DEBUG_ENABLE
		glViewport(0, 0, engine->width, engine->height);
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
		ImGui::SetNextWindowSize(ImVec2(engine->width, engine->height));
		
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

			engine->main_fbo->Bind();
			{
	#endif

	glViewport(0, 0, engine->width, engine->height);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	#ifdef DEBUG_ENABLE
			}
			engine->main_fbo->UnBind();
			ImGui::GetWindowDrawList()->AddImage((void*)engine->main_fbo->id, pos, ImVec2(pos.x + size.x, pos.y + size.y), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

		ImGuiRender();

		ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	#endif

	engine->renderer.RenderPreset();
	SDL_GL_SwapWindow(engine->window);
}

double& ti::TimeStep() {
	return engine->dt;
}

ti::Engine* ti::GetEngine() {
	return engine.get();
}