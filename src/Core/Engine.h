#pragma once

#include <memory>
#include "ECS.h"
#include "Layer.h"

#include <OpenGL.h>
#include <chrono>
#include <iostream>
#include "EngineProperties.h"
#include "Events.h"
#include "EditorLayer.h"


namespace ti {
	class Engine {
		ti::ECS::Registry registry;
		std::vector<std::shared_ptr<Layer>> scene_array;
		ImGuiLayer imguilayer;
		
	public:
		void CreateContext();

		void Init();

		void Mainloop();

		void EventHandler();

		void FixedUpdate(double dt);

		void Update(double dt);
		void Render();

		void Destroy();
};
}