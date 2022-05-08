#pragma once

#include "Enumerations.h"

namespace ti {
	struct WindowContext {
		std::string title = "Game";
		int width = 1280, height = 640, posx = 50, posy = 50;
		WindowType type;

		WindowContext(WindowType type) {
			this->type = type;

		}
	};
};