#pragma once

#include "math/math.h"

struct Material {
	Vec3 ambient;
	Vec3 specular;
	Vec3 diffuse;
	float shininess;
	
	float roughness;
	float diffuse_texture;
	float specular_texture;
};