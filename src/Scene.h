#pragma once

#include "Engine.h"

namespace ti {
	class Scene {
	protected:
		Engine *engine;

	public:
		virtual void Init() = 0;
		virtual void Update(double dt) = 0;
	};
}