#pragma once

#include "../Scene.h"

class Game: public Layer {
public:
	void Init() override;
	void Update(double dt) override;
	void Render() override;
};