#pragma once

#include <string>
#include <SDL2/SDL.h>
#include "Events.h"

namespace ti {
	struct EngineProperties {
		const char* context_title = "Titanium";
		int context_width = 1280, context_height = 640, context_posx = 50, context_posy = 50;

		const char* title = "Titanium";
		int width = 1280, height = 640, posx = 50, posy = 50;

		bool quit = false;
		double dt;


		// status
		int indexcount;
		int vertexcount;
		int drawcalls;
		int framerate;

		bool debug_mode = true;

		SDL_Window* window;
		SDL_GLContext context;
		SDL_Event event;
	};
}
