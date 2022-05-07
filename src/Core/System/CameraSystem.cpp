#include "CameraSystem.h"

void ti::System::CameraSystem::Init(ti::ECS::Registry* registry) {
	this->registry = registry;
}

void ti::System::CameraSystem::Update(double dt) {
	using namespace ti::Component;

	auto& engine = registry->Store<EngineProperties>();
	auto& events = registry->Store<Events>();

	for (auto& entity : registry->View<Transform, Camera>()) {
		if (registry->Contains<Tag>(entity)) continue;

		auto& camera = registry->Get<Camera>(entity);
		auto& transform = registry->Get<Transform>(entity);
		
		if (!camera.enable) continue;

		float distance;

		if (events.key_toggled.contains(SDL_SCANCODE_F4) && camera.type == Editor)
			camera.mode = ORTHOGRAPHIC;
		else
			camera.mode = PERSPECTIVE;

		if (camera.width != engine.width || camera.height != engine.height) {
			camera.width = engine.width;
			camera.height = engine.height;

			camera.lastX = camera.width / 2;
			camera.lastY = camera.height / 2;

			camera.first_mouse = true;

			if (camera.mode == PERSPECTIVE)
				camera.projection = glm::perspective(glm::radians(camera.fov), (float)camera.width / camera.height, camera.camera_near, camera.camera_far);
			if (camera.mode == ORTHOGRAPHIC) {
				int widen = std::tan(glm::radians(camera.fov/2.0)) * camera.camera_far;
				int highten = std::tan(glm::radians((float)(camera.fov / (float)(camera.width/camera.height))/2.0)) * camera.camera_far;

				camera.projection = glm::ortho<float>(-widen, widen, -highten, highten, 0.0, (float)(camera.width/camera.height)*camera.camera_far);
			}
			if (!camera.init) camera.Position = glm::vec3(0, 0, 10);
			camera.init = true;
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
			camera.Position += camera.Front * float(camera.scrollspeed * dt);
		}
		if (events.mouse_scrolldown) {
			camera.Position -= camera.Front * float(camera.scrollspeed * dt);
		}

		distance = glm::length(camera.Position - camera.Center);
		camera.scrollspeed = distance / 10;
		camera.Position = distance * -camera.Front;

		static glm::vec3 delta;
		static glm::vec3 initpos;

		if (events.mouse_pressed.contains(SDL_BUTTON_LEFT) && events.key_pressed.contains(SDL_SCANCODE_LALT)) {
			glm::vec3 pos;
			// if (camera.mode == PERSPECTIVE) {
				glm::vec4 ray_clip = glm::vec4(events.normalized_mouse.x, events.normalized_mouse.y, -1.0, 1.0);
				glm::vec4 ray_eye = glm::inverse(camera.projection) * ray_clip;
				ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
				glm::vec3 ray_wor = glm::inverse(camera.view) * ray_eye;
				ray_wor = glm::normalize(ray_wor);

				float denom = glm::dot(camera.Front, ray_wor);
				float t = -glm::dot((camera.Front * distance), camera.Front) / denom;
				pos = ray_wor * t + (camera.Offset + camera.Position);
			// }
			// if (camera.mode == ORTHOGRAPHIC) {
			// 	int widen = std::tan(glm::radians(camera.fov/2.0)) * camera.camera_far;
			// 	int highten = std::tan(glm::radians((float)(camera.fov / (float)(camera.width/camera.height))/2.0)) * camera.camera_far;

			// 	pos = camera.Offset
			// }

			if (!toggle) {
				initpos = pos;
			}

			delta = pos - initpos;
			toggle = true;
		}

		transform.position = camera.Position + camera.Offset + delta;

		if (camera.mode == PERSPECTIVE)
			camera.view = glm::lookAt(transform.position, transform.position + camera.Front, camera.Up);
		if (camera.mode == ORTHOGRAPHIC) {
			// camera.view = glm::translate(glm::mat4(1.0f), transform.position - camera.Offset) * transform.GetRotationMatrix() * glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0));
		}

		if (toggle && !events.mouse_pressed.contains(SDL_BUTTON_LEFT) && events.key_pressed.contains(SDL_SCANCODE_LALT)) {
			camera.Offset += delta;
			toggle = false;
			delta = glm::vec3();
		}
	}
}

