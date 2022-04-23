#include "Engine.h"

typedef uint8_t MeshTemplates;

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

	renderer.Init();
}

void ti::EventHandler() {
	while (SDL_PollEvent(&engine->event)) {
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

	std::cout << engine->key_chord.size() << '\n';
}

void ti::Update(double dt) {
	auto& registry = engine->registry;
}

void ti::Render() {
	auto& renderer = engine->renderer;
	auto& registry = engine->registry;

	glClearColor(0.2, 0.2, 0.2, 0.2);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SDL_GL_SwapWindow(engine->window);
}

double& ti::TimeStep() {
	return engine->dt;
}

ti::Engine* ti::GetEngine() {
	return engine.get();
}

SDL_Event* ti::GetEvent() {
	return &engine->event;
}