#pragma once

#include "ECS.h"

namespace ti {
	class Layer {
	public:
		ti::ECS::Registry* registry;

		virtual void Init() = 0;
		virtual void Update(double dt) = 0;
	};
}