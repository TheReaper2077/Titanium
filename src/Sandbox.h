#pragma once

#include "Scene.h"

class Sandbox: public Layer {
public:
	void Init() override;
	void Update(double dt) override;
	// void Render() override;
};