#include "Camera.h"

#include "GLFWLayer.h"

Camera::Camera(std::string name, ProjectionMode mode) {
	if (mode == ORTHOGRAPHIC) {
		// this->projection = glm::ortho<float>(-(WIDTH/HEIGHT), (WIDTH/HEIGHT), 1, -1, -1000, 1000);
		// this->projection = glm::ortho<float>(-WIDTH/2, WIDTH/2, HEIGHT/2, -HEIGHT/2, -1000, 1000);
		this->projection = glm::ortho<float>(0, WIDTH, HEIGHT, 0, -1000, 1000);
		this->view = glm::mat4(1.0f);
		this->model = glm::mat4(1.0f);
		this->speed = 5.0f;
	}
	if (mode == PERSPECTIVE) {
		this->projection = glm::perspective(glm::radians(45.0f), (float)WIDTH/HEIGHT, 0.001f, 1000.0f);
		this->view = glm::mat4(1.0f);
		this->model = glm::mat4(1.0f);//glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 0, 1));
		this->speed = 0.5f;
	}

	this->mode = mode;
	this->name = name;
	// this->mouse_enabled = true;
}

void Camera::View(const glm::vec3 &mag) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(mag.x), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(mag.y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(mag.z), glm::vec3(0, 0, 1));

	this->model = model;
}

void Camera::MouseCallback() {
	// float xpos = GetMousePosX();
	// float ypos = GetMousePosY();

	// if (!this->mouse_enabled) {
	// 	this->first_mouse = true;
	// 	return;
	// };

	// if (this->mode == ProjectionMode::PERSPECTIVE) {
	// 	if (this->first_mouse) {
	// 		this->lastX = xpos;
	// 		this->lastY = ypos;
	// 		this->first_mouse = false;
	// 	}

	// 	float xoffset = xpos - this->lastX;
	// 	float yoffset = this->lastY - ypos;
	// 	this->lastX = xpos;
	// 	this->lastY = ypos;

	// 	float sensitivity = 0.1f;
	// 	xoffset *= sensitivity;
	// 	yoffset *= sensitivity;

	// 	this->yaw += xoffset;
	// 	this->pitch += yoffset;

	// 	if (this->pitch > 89.0f)
	// 		this->pitch = 89.0f;
	// 	if (this->pitch < -89.0f)
	// 		this->pitch = -89.0f;

	// 	this->Front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	// 	this->Front.y = sin(glm::radians(this->pitch));
	// 	this->Front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

	// 	this->Front = glm::normalize(this->Front);
	// 	this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
    //     this->Up = glm::normalize(glm::cross(this->Right, this->Front));
	// }
	// if (glfw)
}

void Camera::Update(double dt) {	
	// float speed = (float)this->speed*dt;

	MouseCallback();

	// if (this->mode == PERSPECTIVE) {
	// 	if (!this->keyboard_enabled) return;

	// 	if (IsKeyPressed(GLFW_KEY_W)) {
	// 		this->Position += this->Front * speed;
	// 	}
	// 	if (IsKeyPressed(GLFW_KEY_S)) {
	// 		this->Position -= this->Front * speed;
	// 	}
	// 	if (IsKeyPressed(GLFW_KEY_A)) {
	// 		this->Position -= glm::normalize(glm::cross(this->Front, this->Up)) * speed;
	// 	}
	// 	if (IsKeyPressed(GLFW_KEY_D)) {
	// 		this->Position += glm::normalize(glm::cross(this->Front, this->Up)) * speed;
	// 	}
	// 	if (IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
	// 		this->Position -= this->Up * speed;
	// 	}
	// 	if (IsKeyPressed(GLFW_KEY_SPACE)) {
	// 		this->Position += this->Up * speed;
	// 	}

	// 	this->view = glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	// }

	// if (this->mode == ORTHOGRAPHIC) {	
	// 	if (!this->keyboard_enabled) return;

	// 	// if (IsKeyPressed(GLFW_KEY_W)) {
	// 	// 	this->Position.y -= speed;
	// 	// }
	// 	// if (IsKeyPressed(GLFW_KEY_S)) {
	// 	// 	this->Position.y += speed;
	// 	// }
	// 	// if (IsKeyPressed(GLFW_KEY_A)) {
	// 	// 	this->Position.x -= speed;
	// 	// }
	// 	// if (IsKeyPressed(GLFW_KEY_D)) {
	// 	// 	this->Position.x += speed;
	// 	// }

	// 	this->view = glm::translate(glm::mat4(1.0f), glm::vec3(this->Position.x, this->Position.y, 0));
	// }
}
