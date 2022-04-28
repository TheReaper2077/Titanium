#pragma once

#include "Core/Layer.h"

class Sandbox: public ti::Layer {
public:
	void Init() override;
	void Update(double dt) override;
};