#pragma once

#include "System.h"

namespace ti {
	namespace System {
		class PhysicsSystem {
			ti::ECS::Registry* registry;

			void Init(ti::ECS::Registry* registry) {
				this->registry = registry;
			}

			void Update(double dt) {

			}
		};
	}
}