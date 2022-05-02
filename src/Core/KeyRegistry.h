#pragma once

#include "Engine.h"
#include <unordered_map>
#include <string>

namespace ti {
	struct KeyBindingsRegistry {
		std::unordered_map<std::string, SDL_Scancode> registry;

		KeyBindingsRegistry() {
			Add("MoveUp", SDL_SCANCODE_W);
			Add("MoveDown", SDL_SCANCODE_S);
			Add("MoveLeft", SDL_SCANCODE_A);
			Add("MoveRight", SDL_SCANCODE_D);
		}

		void Add(std::string name, SDL_Scancode scancode) {
			registry[name] = scancode;
		}

		SDL_Scancode Get(std::string name) {
			if (registry.find(name) == registry.end()) {
				return SDL_SCANCODE_UNKNOWN;
			}

			return registry[name];
		}
	};
};