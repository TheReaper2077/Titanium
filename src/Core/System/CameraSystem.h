#pragma once

#include "System.h"


namespace ti {
	namespace System {
		class CameraSystem: public System {
			void Init() override {
				
			}

			void Update(double dt) override {
				using namespace ti::Component;

				for (auto& entity: registry->View<Properties, Transform, ti::Component::Camera>()) {
					auto& camera = registry->Get<ti::Component::Camera>(entity);

					
				}
			}
		}
	}
}