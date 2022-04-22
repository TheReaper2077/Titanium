#include "Engine.h"

typedef uint8_t MeshTemplates;

static std::unique_ptr<Engine> engine;

void CreateContext() {
	engine = std::make_unique<Engine>();

	engine->quit = false;
	
	OpenGL_CreateContext();
	SDL_Init(SDL_INIT_VIDEO);

	engine->window = SDL_CreateWindow("2.5D Engine", 50, 50, engine->width, engine->height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	assert(engine->window);
	engine->context = SDL_GL_CreateContext(engine->window);

	gladLoadGLLoader(SDL_GL_GetProcAddress);
}

void Destroy() {
	
}

void Mainloop() {
	CreateContext();
	Init();

	while (!engine->quit) {
		const auto& start_time = std::chrono::high_resolution_clock::now();

		Update(engine->dt);
		Render();

		const auto& end_time = std::chrono::high_resolution_clock::now();
		engine->dt = std::chrono::duration<double, std::ratio<1, 60>>(end_time - start_time).count();
	}

	Destroy();
}

void Init() {
	auto& registry = engine->registry;
	auto& renderer = engine->renderer;

	renderer.Init();
}

void Update(double dt) {
	auto& registry = engine->registry;
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			engine->quit = true;
		}
	}
}

void Render() {
	auto& renderer = engine->renderer;
	auto& registry = engine->registry;

	glClearColor(0.2, 0.2, 0.2, 0.2);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SDL_GL_SwapWindow(engine->window);
}

double& TimeStep() {
	return engine->dt;
}
