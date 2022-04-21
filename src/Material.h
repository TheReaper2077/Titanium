#pragma once

#include "math/math.h"

struct Material {
	Vec3 ambient;
	Vec3 specular;
	Vec3 diffuse;
	Vec3 roughness;
	int diffuse_texture;
	int specular_texture;
	float shininess;
};