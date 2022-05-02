#pragma once

#include "System.h"


namespace ti {
	namespace System {
		class CameraSystem : public System {
			bool toggle = false;

		public:
			void Init() override {

			}

			void Update(double dt) override {
				using namespace ti::Component;

				auto& engine = registry->Store<EngineProperties>();
				auto& events = registry->Store<Events>();

				for (auto& entity : registry->View<Tag, Transform, Camera>()) {
					auto& camera = registry->Get<Camera>(entity);
					auto& transform = registry->Get<Transform>(entity);

					if (!camera.enable) continue;

					if (camera.mode == PERSPECTIVE) {
						if (camera.width != engine.width || camera.height != engine.height) {
							camera.width = engine.width;
							camera.height = engine.height;

							camera.lastX = camera.width / 2;
							camera.lastY = camera.height / 2;

							camera.first_mouse = true;

							camera.projection = glm::perspective(glm::radians(camera.fov), (float)camera.width / camera.height, camera.near, camera.far);
						}

						if (events.mouse_pressed.contains(SDL_BUTTON_MIDDLE)) {
							if (camera.first_mouse) {
								camera.lastX = events.posx;
								camera.lastY = events.posy;
								camera.first_mouse = false;
							}

							transform.rotation.y -= (events.posx - camera.lastX) * camera.sensitivity;
							transform.rotation.x += (camera.lastY - events.posy) * camera.sensitivity;

							camera.lastX = events.posx;
							camera.lastY = events.posy;
						} else {
							camera.first_mouse = true;
						}

						const auto& quaternion = transform.GetRotationQuat();

						camera.Front = glm::normalize(quaternion * camera.WorldFront);
						camera.Up = glm::normalize(quaternion * camera.WorldUp);

						if (events.mouse_scrollup) {
							transform.position += camera.Front * float(camera.scrollspeed * dt);
						}
						if (events.mouse_scrolldown) {
							transform.position -= camera.Front * float(camera.scrollspeed * dt);
						}

						auto distance = glm::length(transform.position - camera.Center);
						transform.position = distance * -camera.Front;

						static glm::vec3 delta;
						static glm::vec3 initpos;
						
						if (events.mouse_pressed.contains(SDL_BUTTON_LEFT) && events.key_pressed.contains(SDL_SCANCODE_LALT)) {
							glm::vec4 ray_clip = glm::vec4(events.normalized_mouse.x, events.normalized_mouse.y, -1.0, 1.0);
							glm::vec4 ray_eye = glm::inverse(camera.projection) * ray_clip;
							ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
							glm::vec3 ray_wor = glm::inverse(camera.view) * ray_eye;
							ray_wor = glm::normalize(ray_wor);
							
							float denom = glm::dot(camera.Front, ray_wor);
							float t = -glm::dot((camera.Front * distance), camera.Front) / denom;
							glm::vec3 pos = ray_wor * t + (camera.Offset + transform.position);

							if (!toggle) {
								initpos = pos;
							}
							
							delta = pos - initpos;
							toggle = true;
						}

						camera.view = glm::lookAt(transform.position + camera.Offset + delta, transform.position + camera.Front + camera.Offset + delta, camera.Up);
						
						if (toggle && !events.mouse_pressed.contains(SDL_BUTTON_LEFT) && events.key_pressed.contains(SDL_SCANCODE_LALT)) {
							camera.Offset += delta;
							toggle = false;
							delta = glm::vec3();
						}
					}
				}
			}
		};
	}
}
