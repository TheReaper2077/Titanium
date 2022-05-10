#include "Engine.h"
#include "WindowRegistry.h"
#include "System/CameraSystem.h"
#include "System/RenderSystem.h"
#include "System/Physics.h"

ti::System::RenderSystem rendersystem;
ti::System::CameraSystem camerasystem;
ti::System::Physics physicssystem;

void ti::Engine::CreateContext() {
	auto& engine = registry.Store<EngineProperties>();
	registry.Store<ti::Functions>(&registry);
	registry.Store<ti::WindowRegistry>(&registry);

	engine.quit = false;

	OpenGL_CreateContext();
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	engine.window = SDL_CreateWindow(engine.context_title, engine.context_posx, engine.context_posy, engine.context_width, engine.context_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	assert(engine.window);
	engine.context = SDL_GL_CreateContext(engine.window);

	gladLoadGLLoader(SDL_GL_GetProcAddress);

	registry.Store<WindowRegistry>().Create(GameWindow);
	if (engine.debug_mode) {
		registry.Store<WindowRegistry>().Create(EditorWindow);

		imguilayer.registry = &registry;
		imguilayer.Init();
	}
}

void ti::Engine::Init() {
	auto& engine = registry.Store<EngineProperties>();

	registry.Store<ti::Functions>().ImportModel("D:\\C++\\2.5D Engine\\res\\cube.obj");
	registry.Store<ti::Functions>().ImportModel("D:\\C++\\2.5D Engine\\res\\cylinder.obj");

	SDL_ShowCursor(SDL_DISABLE);

	registry.Store<ti::Functions>().LoadEntities();
	
	rendersystem.Init(&registry);
	camerasystem.Init(&registry);
	physicssystem.Init(&registry);
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

		Update(engine.dt);
		Render();

		SDL_GL_SwapWindow(engine.window);

		const auto& end_time = std::chrono::high_resolution_clock::now();
		engine.dt = std::chrono::duration<double, std::ratio<1, 60>>(end_time - start_time).count();
	}

	Destroy();
}

void ti::Engine::EventHandler() {
	auto& events = registry.Store<Events>();
	auto& engine = registry.Store<EngineProperties>();
	auto& editorwindow = registry.Store<WindowRegistry>().Get(EditorWindow);

	events.mouse_scrollup = false;
	events.mouse_scrolldown = false;
	static bool in_window;

	while (SDL_PollEvent(&engine.event)) {
		imguilayer.EventHandler();

		if (engine.event.type == SDL_QUIT) {
			engine.quit = true;
		}
		if (engine.event.type == SDL_MOUSEMOTION) {
			in_window = (editorwindow.focused && engine.event.motion.x >= editorwindow.posx && engine.event.motion.x < editorwindow.width + editorwindow.posx && engine.event.motion.y >= editorwindow.posy && engine.event.motion.y < editorwindow.height + editorwindow.posy);

			if (in_window) {
				events.posx += engine.event.motion.xrel;
				events.posy += engine.event.motion.yrel;
				
				events.relx = engine.event.motion.xrel;
				events.rely = engine.event.motion.yrel;

				events.editor_normalized_mouse.x = ((engine.event.motion.x - editorwindow.posx) / (editorwindow.width * 0.5)) - 1.0;
				events.editor_normalized_mouse.y = 1.0 - ((engine.event.motion.y - editorwindow.posy) / (editorwindow.height * 0.5));
				events.editor_normalized_mouse.z = 0;

				events.editor_mouspos.x = (1 + events.editor_normalized_mouse.x) * editorwindow.width / 2.0;
				events.editor_mouspos.y = (1 + events.editor_normalized_mouse.y) * editorwindow.height / 2.0;

				// events.game_normalized_mouse.x = ((engine.event.motion.x - engine.game_posx) / (engine.game_width * 0.5)) - 1.0;
				// events.game_normalized_mouse.y = 1.0 - ((engine.event.motion.y - engine.game_posy) / (engine.game_height * 0.5));
				// events.game_normalized_mouse.z = 0;

				// events.game_mouspos.x = (1 + events.game_normalized_mouse.x) * engine.game_width / 2.0;
				// events.game_mouspos.y = (1 + events.game_normalized_mouse.y) * engine.game_height / 2.0;
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
	auto& events = registry.Store<ti::Events>();

	if (events.key_pressed.contains(SDL_SCANCODE_S) && events.key_pressed.contains(SDL_SCANCODE_LCTRL)) {
		registry.Store<ti::Functions>().SaveEntities();
	}

	if (engine.play && !engine.pause && !engine.stop) {
		physicssystem.Update(dt);
	}
	
	if (engine.stop) {
		engine.stop = false;
		registry.Store<ti::Functions>().LoadEntities();
	}

	camerasystem.Update(dt);
}

void ti::Engine::Render() {
	auto& engine = registry.Store<EngineProperties>();

	engine.drawcalls = 0;
	engine.indexcount = 0;
	engine.vertexcount = 0;
	
	rendersystem.Render(registry.Store<WindowRegistry>().Get(GameWindow));
	if (engine.debug_mode) {
		rendersystem.Render(registry.Store<WindowRegistry>().Get(EditorWindow));
		imguilayer.Render();
	}
	
	static int count;
	static double time_seconds;

	if (time_seconds >= 60) {
		engine.framerate = count;
		count = 0;
		time_seconds = 0;
	}

	count++;
	time_seconds += engine.dt;
}

void ti::Engine::Destroy() {

}

