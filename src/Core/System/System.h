#pragma once

#include "../ECS.h"
#include "../Components/Components.h"

namespace ti {
	namespace System {
		class System {
		public:
			ti::ECS::Registry *registry;

			virtual void Init() = 0;
			virtual void Update(double dt) = 0;
			virtual void FixedUpdate(double dt) = 0;
		};
	}
}