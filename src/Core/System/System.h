#pragma once

#include "../Engine.h"
#include "../ECS.h"
#include "../Components/Components.h"

namespace ti {
	namespace System {
		class System {
		public:
			ti::ECS::Registry *registry;

			virtual void Init(ti::ECS::Registry *registry) = 0;
			virtual void Update(double dt) = 0;
			// virtual void FixedUpdate(double dt) = 0;
		};
	}
}

// #include "RenderSystem.h"
// #include "CameraSystem.h"