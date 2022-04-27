#pragma once

// #ifndef ENGINE
#include "Renderer.h"
#include "ECS.h"
#include "Events.h"
// #endif

class Layer {
public:
	ti::Renderer* renderer;
	ti::ECS::Registry* registry;
	int width, height;
	ti::Events* eventdata;

public:
	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	// virtual void Render() = 0;
};
