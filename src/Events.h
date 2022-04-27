#pragma once

#include <set>
#include <SDL2/SDL_events.h>

namespace ti {
	struct Events {
		bool mouse_scrollup;
		bool mouse_scrolldown;
		
		int relmouseposx = 0;
		int relmouseposy = 0;
		int mouseposx = 0;
		int mouseposy = 0;

		std::set<SDL_Scancode> key_pressed;
		std::vector<SDL_Scancode> key_chord;

		std::set<unsigned int> mouse_pressed;
	};
}