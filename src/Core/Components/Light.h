#pragma once

#include <glm/glm.hpp>

namespace ti {
	namespace Component {
		struct Light {
			glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
			glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
			glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
		
			float constant = 1.0;
			float linear = 0.09;
			float quadratic = 0.032;
			
			float cutOff = 0;
			float outerCutOff = 0;

			LightMode mode = Directional;

			bool active = true;

			Light() {}
		};
	}
}