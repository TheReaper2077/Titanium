#pragma once

#include "math/math.h"

enum LightType {
	POINT,
	DIRECTIONAL,
	SPOTLIGHT
};

struct Light {
    Vec4 position;
  
    Vec3 ambient;
    Vec3 diffuse;
    Vec3 specular;

	// Point Light
	float constant;
    float linear;
    float quadratic;

	// Spotlight
	float cutOff;
};