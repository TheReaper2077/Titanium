#pragma once

#include "Components/Components.h"
#include "WindowContext.h"

namespace ti {
	struct WindowRegistry {
		std::unordered_map<WindowType, WindowContext> windowregistry;
	private:
		// std::unordered_map<WindowType, std::string> key_registry;
		ECS::Registry* registry;

	public:
		WindowRegistry() {}
		WindowRegistry(ECS::Registry* registry): registry(registry) {}

		void Create(WindowType type) {
			WindowContext window;
			window.type = type;
			window.camera = 0;
			window.framebuffer = FrameBuffer_Create(window.width, window.height, (type == EditorWindow));
			
			if (type == EditorWindow) {
				window.title = "Editor";
			}
			if (type == GameWindow) {
				window.title = "Game";
			}

			windowregistry[type] = window;
		}

		WindowContext& Get(WindowType type) {
			assert(windowregistry.find(type) != windowregistry.end());

			return windowregistry[type];
		}
	};
};