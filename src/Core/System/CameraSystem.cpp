#include "CameraSystem.h"
#include "../WindowRegistry.h"

void ti::System::CameraSystem::Init(ti::ECS::Registry* registry) {
	this->registry = registry;
}

void ti::System::CameraSystem::EditorCamera() {
	using namespace ti::Component;

	auto& events = registry->Store<ti::Events>();
	auto& engine = registry->Store<EngineProperties>();
	auto& window = registry->Store<ti::WindowRegistry>().Get(EditorWindow);

	if (!engine.debug_mode) return;

	if (window.camera == 0) {
		window.camera = registry->Create();
		registry->Add<Transform>(window.camera);
		registry->Add<Camera>(window.camera, PERSPECTIVE, true, Default);
	}

	auto& camera = registry->Get<Camera>(window.camera);
	auto& transform = registry->Get<Transform>(window.camera);

	float distance;

	if (camera.width != window.width || camera.height != window.height) {
		camera.width = window.width;
		camera.height = window.height;

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
		camera.Position += camera.Front * float(camera.scrollspeed * engine.dt);
	}
	if (events.mouse_scrolldown) {
		camera.Position -= camera.Front * float(camera.scrollspeed * engine.dt);
	}

	distance = glm::length(camera.Position - camera.Center);
	camera.scrollspeed = distance / 10;
	camera.Position = distance * -camera.Front;

	static glm::vec3 delta;
	static glm::vec3 initpos;

	if (events.mouse_pressed.contains(SDL_BUTTON_LEFT) && events.key_pressed.contains(SDL_SCANCODE_LALT)) {
		glm::vec3 pos;
		glm::vec4 ray_clip = glm::vec4(events.editor_normalized_mouse.x, events.editor_normalized_mouse.y, -1.0, 1.0);
		glm::vec4 ray_eye = glm::inverse(camera.projection) * ray_clip;
		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
		glm::vec3 ray_wor = glm::inverse(camera.view) * ray_eye;
		ray_wor = glm::normalize(ray_wor);

		float denom = glm::dot(camera.Front, ray_wor);
		float t = -glm::dot((camera.Front * distance), camera.Front) / denom;
		pos = ray_wor * t + (camera.Offset + camera.Position);

		if (!toggle) {
			initpos = pos;
		}

		delta = pos - initpos;
		toggle = true;
	}

	transform.position = camera.Position + camera.Offset + delta;

	camera.view = glm::lookAt(transform.position, transform.position + camera.Front, camera.Up);

	if (toggle && !events.mouse_pressed.contains(SDL_BUTTON_LEFT) && events.key_pressed.contains(SDL_SCANCODE_LALT)) {
		camera.Offset += delta;
		toggle = false;
		delta = glm::vec3();
	}
}

void ti::System::CameraSystem::GameCamera() {
	using namespace ti::Component;
	auto& events = registry->Store<ti::Events>();
	auto& engine = registry->Store<EngineProperties>();
	auto& window = registry->Store<ti::WindowRegistry>().Get(GameWindow);

	if (window.camera == 0) {
		window.camera = registry->Create();
		registry->Add<Transform>(window.camera);
		registry->Add<Camera>(window.camera, PERSPECTIVE, true, Default);
	}

	auto& camera = registry->Get<Camera>(window.camera);
	auto& transform = registry->Get<Transform>(window.camera);
}

void ti::System::CameraSystem::Update(double dt) {
	EditorCamera();
}

