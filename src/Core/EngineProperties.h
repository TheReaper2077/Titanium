#pragma once

#include <string>
#include <SDL2/SDL.h>
#include "Events.h"

namespace ti {
	struct EngineProperties {
		const char* context_title = "Titanium";
		int context_width = 1280, context_height = 640, context_posx = 50, context_posy = 50;

		const char* editor_title = "Editor";
		int editor_width = 1280, editor_height = 640, editor_posx = 50, editor_posy = 50;

		const char* game_title = "Game";
		int game_width = 1280, game_height = 640, game_posx = 50, game_posy = 50;

		bool quit = false;
		double dt;


		// status
		int indexcount;
		int vertexcount;
		int drawcalls;
		int framerate;

		bool debug_mode = true;
		bool debug_render = false;

		SDL_Window* window;
		SDL_GLContext context;
		SDL_Event event;

		bool play;
		bool stop;
		bool pause;
	};
}
