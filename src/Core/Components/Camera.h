#pragma once

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_cross_product.hpp>

namespace ti {
	enum Projection {
		ORTHOGRAPHIC,
		PERSPECTIVE
	};

	namespace Component {
		struct Camera {
			glm::mat4 projection;
			glm::mat4 view;
			glm::mat4 model;

			glm::vec3 Up;
			glm::vec3 Right;
			glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

			Projection mode;

			bool enable = false;

			bool first_mouse = true;
			float speed = 0;
			float yaw = -90.0, pitch = 0;
			float lastX, lastY;
			float zoom = 0.001f;
			bool init = false;

			Camera() {}
			Camera(Projection mode, float width, float height, bool enable = false) {
				this->enable = enable;

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
					this->speed = 0.005f;
				}

				this->mode = mode;

				this->lastX = width/2;
				this->lastY = height/2;
			}
		};
	}
}