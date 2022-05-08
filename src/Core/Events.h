#pragma once

#include <set>
#include <vector>
#include <SDL2/SDL_events.h>
#include <glm/glm.hpp>

namespace ti {
	struct Events {
		bool mouse_scrollup;
		bool mouse_scrolldown;
		
		int relx = 0;
		int rely = 0;
		int posx = 0;
		int posy = 0;

		std::set<SDL_Scancode> key_pressed;
		std::vector<SDL_Scancode> key_chord;
		std::set<SDL_Scancode> key_toggled;

		glm::vec3 editor_normalized_mouse;
		glm::vec2 editor_mouspos;

		glm::vec3 game_normalized_mouse;
		glm::vec2 game_mouspos;

		std::set<unsigned int> mouse_pressed;
	};
}