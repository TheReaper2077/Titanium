#pragma once

#include "System.h"


namespace ti {
	namespace System {
		class CameraSystem: public System {
		public:
			void Init() override {
				
			}

			void Update(double dt) override {
				using namespace ti::Component;

				auto& engine = registry->Store<EngineProperties>();
				auto& events = registry->Store<Events>();

				for (auto& entity: registry->View<Properties, Transform, ti::Component::Camera>()) {
					auto& camera = registry->Get<ti::Component::Camera>(entity);
					auto& transform = registry->Get<ti::Component::Transform>(entity);

					if (camera.mode == Projection::PERSPECTIVE) {
						if (camera.first_mouse) {
							camera.lastX = events.posx;
							camera.lastY = events.posy;
							camera.first_mouse = false;
						}

						float xoffset = events.posx - camera.lastX;
						float yoffset = camera.lastY - events.posy;
						camera.lastX = events.posx;
						camera.lastY = events.posy;

						float sensitivity = 0.1f;
						xoffset *= sensitivity;
						yoffset *= sensitivity;

						camera.yaw += xoffset;
						camera.pitch += yoffset;

						if (camera.pitch > 89.0f)
							camera.pitch = 89.0f;
						if (camera.pitch < -89.0f)
							camera.pitch = -89.0f;

						camera.Front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
						camera.Front.y = sin(glm::radians(camera.pitch));
						camera.Front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));

						camera.Front = glm::normalize(camera.Front);
						camera.Right = glm::normalize(glm::cross(camera.Front, camera.WorldUp));
						camera.Up = glm::normalize(glm::cross(camera.Right, camera.Front));

						if (events.key_pressed.contains(SDL_SCANCODE_W)) {
							transform.position += camera.Front * camera.speed;
						}
						if (events.key_pressed.contains(SDL_SCANCODE_S)) {
							transform.position -= camera.Front * camera.speed;
						}
						if (events.key_pressed.contains(SDL_SCANCODE_A)) {
							transform.position -= glm::normalize(glm::cross(camera.Front, camera.Up)) * camera.speed;
						}
						if (events.key_pressed.contains(SDL_SCANCODE_D)) {
							transform.position += glm::normalize(glm::cross(camera.Front, camera.Up)) * camera.speed;
						}
						if (events.key_pressed.contains(SDL_SCANCODE_LSHIFT)) {
							transform.position -= camera.Up * camera.speed;
						}
						if (events.key_pressed.contains(SDL_SCANCODE_SPACE)) {
							transform.position += camera.Up * camera.speed;
						}

						camera.view = glm::lookAt(transform.position, transform.position + camera.Front, camera.Up);
					}
				}
			}
		};
	}
}