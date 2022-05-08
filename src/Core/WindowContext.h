#pragma once

#include <OpenGL.h>
#include "Enumerations.h"

namespace ti {
	struct WindowContext {
		std::string title = "Game";
		int width = 1280, height = 640, posx = 50, posy = 50;
		WindowType type;

		glm::vec2 pos;
		glm::vec2 pos_normalized;

		WindowContext(WindowType type) {
			this->type = type;

		}
	};
};