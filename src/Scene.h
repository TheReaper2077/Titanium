#pragma once

// #ifndef ENGINE
#include "Renderer.h"
#include "ECS.h"
// #endif

class Layer {
public:
	ti::Renderer* renderer;
	ti::ECS::Registry* registry;
	int width, height;

public:
	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
};
