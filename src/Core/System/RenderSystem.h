#pragma once

#include "System.h"

namespace ti {
	namespace System {
		class RenderSystem: System {
			using namespace ti::Component;

			void Init() override {

			}

			void Update(double dt) override {
				for (auto& entity: registry->View<MeshCollider>()) {
					
				}
			}
		};
	}
}