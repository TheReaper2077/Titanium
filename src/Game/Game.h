#pragma once

#include "../Scene.h"

class Game: public Scene {
	void Init() override;
	void Update(double dt) override;
}