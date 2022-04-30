#pragma once

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_cross_product.hpp>
#include "../Enumerations.h"

namespace ti {
	namespace Component {
		struct Camera {
			glm::mat4 projection;
			glm::mat4 view;

			glm::vec3 Up;
			glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::vec3 WorldFront = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 Center = glm::vec3(0, 0, 0);
			glm::vec3 Offset = glm::vec3(0, 0, 0);

			Projection mode;
			CameraType type;

			bool enable = false;

			bool first_mouse = true;
			float speed = 0;
			float scrollspeed = 0;
			float lastX, lastY;
			float sensitivity = 0.1f;

			int initx, inity;

			int width = 0;
			int height = 0;
			float fov = 45.0;
			float near = 0.001;
			float far = 1000.0;

			bool changed = true;

			Camera() {}
			Camera(Projection mode, bool enable = false, CameraType type = Editor) {
				this->enable = enable;
				this->mode = mode;
				this->type = type;
				this->speed = 0.256f;
				this->scrollspeed = 0.750f;
				this->sensitivity = 0.5f;
			}
		};
	}
}