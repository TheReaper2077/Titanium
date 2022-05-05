#pragma once

#include "System.h"
#include "../KeyRegistry.h"

namespace ti {
	namespace System {
		class CameraSystem {
			bool toggle = false;
			ti::ECS::Registry *registry;

		public:
			void Init(ti::ECS::Registry *registry);

			void Update(double dt);
		};
	}
}
