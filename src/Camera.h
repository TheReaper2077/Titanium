#pragma once

// #include <glm/glm.hpp>
#include "define.h"

enum ProjectionMode {
	ORTHOGRAPHIC,
	PERSPECTIVE
};

struct Camera {
	ProjectionMode mode;

	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	bool first_mouse = true;
	float speed = 0;
	float yaw = -90.0, pitch = 0;
	float lastX = WIDTH/2, lastY = HEIGHT/2;
	float zoom = 0.001f;

	bool mouse_enabled = false;
	bool keyboard_enabled = true;

	std::string name;

	void MouseCallback();
	void View(const glm::vec3 &mag);
	void View(float zoom);
	void Update(double dt);

	Camera(std::string name, ProjectionMode mode);
};
