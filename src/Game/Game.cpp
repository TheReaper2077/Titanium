#include "Game.h"
#include "Camera.h"

#include <iostream>

ti::Camera *camera;

void Game::Init() {
	camera = new ti::Camera("cam0", ti::ProjectionMode::ORTHOGRAPHIC, width, height);
}

void Game::Update(double dt) {
	camera->Update(dt);
}

void Game::Render() {
	renderer->SetCamera(camera);
	renderer->SetShader("color");

	glViewport(0, 0, this->width, this->height);
	glClearColor(0.2, 0.2, 0.2, 0.2);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderer->DrawRect(0, 0, 0.5, 0.5);
}
