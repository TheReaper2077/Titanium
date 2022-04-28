#include "Camera.h"

ti::Camera::Camera(std::string name, ProjectionMode mode, int width, int height) {
	if (mode == ORTHOGRAPHIC) {
		this->projection = glm::ortho<float>(-(width/height)*5, (width/height)*5, -1*5, 1*5, -1000, 1000);
		// this->projection = glm::ortho<float>(0, width, height, 0, -1000, 1000);
		this->view = glm::mat4(1.0f);
		this->model = glm::mat4(1.0f);
		this->speed = 5.0f;
	}
	if (mode == PERSPECTIVE) {
		this->projection = glm::perspective(glm::radians(45.0f), (float)width/height, 0.001f, 1000.0f);
		this->view = glm::mat4(1.0f);
		this->model = glm::mat4(1.0f);
		this->speed = 0.5f;
	}

	this->mode = mode;
	this->name = name;

	this->lastX = width/2;
	this->lastY = height/2;
}

void ti::Camera::View(const glm::vec3 &mag) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(mag.x), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(mag.y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(mag.z), glm::vec3(0, 0, 1));

	this->model = model;
}

void ti::Camera::MouseCallback(Events *eventdata) {
	// if (!this->mouse_enabled) {
	// 	this->first_mouse = true;
	// 	return;
	// };

	if (this->mode == ProjectionMode::PERSPECTIVE) {
		if (this->first_mouse) {
			this->lastX = eventdata->posx;
			this->lastY = eventdata->posy;
			this->first_mouse = false;
		}

		float xoffset = eventdata->posx - this->lastX;
		float yoffset = this->lastY - eventdata->posy;
		this->lastX = eventdata->posx;
		this->lastY = eventdata->posy;
		
		// float xoffset = eventdata->relmouseposx;
		// float yoffset = eventdata->relmouseposy;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		this->yaw += xoffset;
		this->pitch += yoffset;

		if (this->pitch > 89.0f)
			this->pitch = 89.0f;
		if (this->pitch < -89.0f)
			this->pitch = -89.0f;

		this->Front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->Front.y = sin(glm::radians(this->pitch));
		this->Front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

		this->Front = glm::normalize(this->Front);
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
        this->Up = glm::normalize(glm::cross(this->Right, this->Front));
	}
}

void ti::Camera::Update(double dt, Events *eventdata) {
	float speed = (float)this->speed*dt;

	MouseCallback(eventdata);

	if (this->mode == PERSPECTIVE) {
		if (eventdata->key_pressed.contains(SDL_SCANCODE_W)) {
			this->Position += this->Front * speed;
		}
		if (eventdata->key_pressed.contains(SDL_SCANCODE_S)) {
			this->Position -= this->Front * speed;
		}
		if (eventdata->key_pressed.contains(SDL_SCANCODE_A)) {
			this->Position -= glm::normalize(glm::cross(this->Front, this->Up)) * speed;
		}
		if (eventdata->key_pressed.contains(SDL_SCANCODE_D)) {
			this->Position += glm::normalize(glm::cross(this->Front, this->Up)) * speed;
		}
		if (eventdata->key_pressed.contains(SDL_SCANCODE_LSHIFT)) {
			this->Position -= this->Up * speed;
		}
		if (eventdata->key_pressed.contains(SDL_SCANCODE_SPACE)) {
			this->Position += this->Up * speed;
		}

		this->view = glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	}

	if (this->mode == ORTHOGRAPHIC) {	
		// if (!this->keyboard_enabled) return;

		// if (IsKeyPressed(GLFW_KEY_W)) {
		// 	this->Position.y -= speed;
		// }
		// if (IsKeyPressed(GLFW_KEY_S)) {
		// 	this->Position.y += speed;
		// }
		// if (IsKeyPressed(GLFW_KEY_A)) {
		// 	this->Position.x -= speed;
		// }
		// if (IsKeyPressed(GLFW_KEY_D)) {
		// 	this->Position.x += speed;
		// }

		this->view = glm::translate(glm::mat4(1.0f), glm::vec3(this->Position.x, this->Position.y, 0));
	}
}
