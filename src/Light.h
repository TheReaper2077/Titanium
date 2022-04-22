#pragma once

#include "math/math.h"

struct Light{};

struct DirLight: Light {
	Vec3 direction;

	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
};

struct PointLight: Light {
	Vec3 position;

	float constant;
	float linear;
	float quadratic;

	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
};

struct SpotLight: Light {
	Vec3 position;
	Vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;       
};

struct AreaLight: Light {
	Vec3 position;
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;   
};
