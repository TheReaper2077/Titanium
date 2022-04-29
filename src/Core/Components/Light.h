#pragma once

#include <glm/glm.hpp>

namespace ti {
	namespace Component {
		struct Light {
			glm::vec3 position;  
			glm::vec3 direction;
		
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
		
			float constant;
			float linear;
			float quadratic;
			
			float cutOff;
			float outerCutOff;

			LightMode mode;
		};
	}
}