#pragma once

#include <OpenGL.h>
#include "Enumerations.h"
#include "ECS.h"

namespace ti {
	struct WindowContext {
		std::string title = "Titanium";
		int width = 1280, height = 640, posx = 50, posy = 50;
		WindowType type;

		glm::vec2 pos;
		glm::vec2 pos_normalized;

		ECS::Entity camera;

		FrameBuffer* framebuffer;
	};
};