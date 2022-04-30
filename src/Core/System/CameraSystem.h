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
							transform.position += camera.Front * camera.scrollspeed;
						}
						if (events.mouse_scrolldown) {
							transform.position -= camera.Front * camera.scrollspeed;
						}

						if (events.key_pressed.contains(SDL_SCANCODE_A)) {
							camera.Offset -= glm::normalize(glm::cross(camera.Front, camera.Up)) * camera.speed;
						}
						if (events.key_pressed.contains(SDL_SCANCODE_D)) {
							camera.Offset += glm::normalize(glm::cross(camera.Front, camera.Up)) * camera.speed;
						}

						auto distance = glm::length(transform.position - camera.Center);
						transform.position = distance * -camera.Front;
						
						// if (events.key_pressed.contains(SDL_SCANCODE_W)) {
						// 	transform.position += camera.Front * camera.speed;
						// }
						// if (events.key_pressed.contains(SDL_SCANCODE_S)) {
						// 	transform.position -= camera.Front * camera.speed;
						// }
						// if (events.key_pressed.contains(SDL_SCANCODE_LSHIFT)) {
						// 	transform.position -= camera.Up * camera.speed;
						// }
						// if (events.key_pressed.contains(SDL_SCANCODE_SPACE)) {
						// 	transform.position += camera.Up * camera.speed;
						// }

						camera.view = glm::lookAt(transform.position + camera.Offset, transform.position + camera.Front + camera.Offset, camera.Up);
					}
				}
			}
		};
	}
}

// void Camera::SetViewByMouse(void) {
// 	// the coordinates of our mouse coordinates
// 	int MouseX, MouseY;
// 	// the middle of the screen in the x direction 
// 	int MiddleX = SCREENWIDTH / 2; // the middle of the screen in the y direction 
// 	int MiddleY = SCREENHEIGHT / 2; // vector that describes mouseposition - center 
// 	Vector MouseDirection(0, 0, 0); // static variable to store the rotation about the x-axis, since 
// 	// we want to limit how far up or down we can look. 
// 	// We don't need to cap the rotation about the y-axis as we 
// 	// want to be able to turn around 360 degrees 
// 	static double CurrentRotationAboutX = 0.0;
// 	// The maximum angle we can look up or down, in radians 
// 	double maxAngle = 1; // This function gets the position of the mouse 
// 	SDL_GetMouseState(&MouseX, &MouseY); // if the mouse hasn't moved, return without doing 
// 	// anything to our view 
// 	if ((MouseX == MiddleX) && (MouseY == MiddleY)) return;
// 	// otherwise move the mouse back to the middle of the screen 
// 	SDL_WarpMouse(MiddleX, MiddleY); // get the distance and direction the mouse moved in x (in 
// 	// pixels). We can't use the actual number of pixels in radians, 
// 	// as only six pixels would cause a full 360 degree rotation. 
// 	// So we use a mousesensitivity variable that can be changed to 
// 	// vary how many radians we want to turn in the x-direction for 
// 	// a given mouse movement distance 
// 	// We have to remember that positive rotation is counter-clockwise. 
// 	// Moving the mouse down is a negative rotation about the x axis 
// 	// Moving the mouse right is a negative rotation about the y axis 
// 	MouseDirection.x = (MiddleX - MouseX) / MouseSensitivity;
// 	MouseDirection.y = (MiddleY - MouseY) / MouseSensitivity;
// 	CurrentRotationX += MouseDirection.y;
// 	// We don't want to rotate up more than one radian, so we cap it. 
// 	if (CurrentRotationX > 1) { CurrentRotationX = 1; return; }
// 	// We don't want to rotate down more than one radian, so we cap it. 
// 	if (CurrentRotationX < -1) { CurrentRotationX = -1; return; } else {
// 		// get the axis to rotate around the x-axis. 
// 		Vector Axis = CrossProduct(View - Position, Up);
// 		// To be able to use the quaternion conjugate, the axis to 
// 		// rotate around must be normalized. 
// 		Axis = Normalize(Axis);
// 		// Rotate around the y axis 
// 		RotateCamera(MouseDirection.y, Axis.x, Axis.y, Axis.z); // Rotate around the x axis 
// 		RotateCamera(MouseDirection.x, 0, 1, 0);
// 	}
// }
