#pragma once

#include "../ECS.h"
#include "Components/Components.h"

namespace ti {
	namespace System {
		class System {
			ti::ECS::Registry *registry;

			virtual void Init() = 0;
			virtual void Update() = 0;
		};
	}
}