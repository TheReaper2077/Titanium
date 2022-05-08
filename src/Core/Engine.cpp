#include "Engine.h"
#include "System/CameraSystem.h"
#include "System/RenderSystem.h"

ti::System::RenderSystem rendersystem;
ti::System::CameraSystem camerasystem;

void ti::Engine::CreateContext() {
	auto& engine = registry.Store<EngineProperties>();
	registry.Store<ti::Functions>(&registry);

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

void ti::Engine::Init() {
	auto& engine = registry.Store<EngineProperties>();

	registry.Store<ti::Functions>().ImportModel("D:\\C++\\2.5D Engine\\res\\cube.obj");
	registry.Store<ti::Functions>().ImportModel("D:\\C++\\2.5D Engine\\res\\cylinder.obj");

	SDL_ShowCursor(SDL_DISABLE);

	registry.Store<ti::Functions>().LoadEntities();
	
	rendersystem.Init(&registry);
	camerasystem.Init(&registry);
}

void ti::Engine::Mainloop() {
	Init();

	auto& events = registry.Store<Events>();
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

void ti::Engine::EventHandler() {
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
			if (events.key_pressed.contains(engine.event.key.keysym.scancode)) {
				if (!events.key_toggled.contains(engine.event.key.keysym.scancode))
					events.key_toggled.insert(engine.event.key.keysym.scancode);
				else
					events.key_toggled.erase(engine.event.key.keysym.scancode);
			}
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

void ti::Engine::FixedUpdate(double dt) {

}

void ti::Engine::Update(double dt) {
	auto& engine = registry.Store<EngineProperties>();

	engine.drawcalls = 0;
	engine.indexcount = 0;
	engine.vertexcount = 0;

	auto& events = registry.Store<ti::Events>();

	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glEnable(GL_CULL_FACE);`
	glViewport(0, 0, engine.width, engine.height);
	glClearColor(0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (events.key_pressed.contains(SDL_SCANCODE_S) && events.key_pressed.contains(SDL_SCANCODE_LCTRL)) {
		registry.Store<ti::Functions>().SaveEntities();
	}

	camerasystem.Update(dt);

	rendersystem.SetRenderPass(true);
	rendersystem.Update(dt);
	rendersystem.SetRenderPass(false);

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

void ti::Engine::Destroy() {

}

