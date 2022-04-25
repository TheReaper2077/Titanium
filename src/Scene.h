#pragma once

// #ifndef ENGINE
#include "Renderer.h"
// #endif

class Layer {
public:
	ti::Renderer* renderer;
	int width, height;

public:
	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
};
