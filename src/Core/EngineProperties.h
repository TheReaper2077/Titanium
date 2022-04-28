#pragma once

#include <string>
#include <SDL2/SDL.h>
#include "Events.h"

namespace ti {
	struct EngineProperties {
		const char* title = "Titanium";
		int width = 1280, height = 640, posx = 50, posy = 50;

		bool quit = false;
		double dt;

		SDL_Window* window;
		SDL_GLContext context;
		SDL_Event event;
	};
}
