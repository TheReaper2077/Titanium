#pragma once

#include "Enumerations.h"
#include "MeshRegistry.h"
#include "Components/Components.h"
#include "assert.h"

static ti::Component::Mesh CubeModel() {
	ti::Component::Mesh mesh;

	mesh.primitive = ti::Primitive::TRIANGLE;

	mesh.positions = {
		{ -0.5, -0.5, -0.5 },
		{ -0.5, +0.5, -0.5 },
		{ +0.5, +0.5, -0.5 },
		{ +0.5, -0.5, -0.5 },

		{ -0.5, -0.5, +0.5 },
		{ -0.5, +0.5, +0.5 },
		{ +0.5, +0.5, +0.5 },
		{ +0.5, -0.5, +0.5 },

		{ -0.5, -0.5, +0.5 },
		{ -0.5, +0.5, +0.5 },
		{ -0.5, +0.5, -0.5 },
		{ -0.5, -0.5, -0.5 },

		{ +0.5, -0.5, +0.5 },
		{ +0.5, +0.5, +0.5 },
		{ +0.5, +0.5, -0.5 },
		{ +0.5, -0.5, -0.5 },
		
		{ -0.5, -0.5, -0.5 },
		{ -0.5, -0.5, +0.5 },
		{ +0.5, -0.5, +0.5 },
		{ +0.5, -0.5, -0.5 },

		{ -0.5, +0.5, -0.5 },
		{ -0.5, +0.5, +0.5 },
		{ +0.5, +0.5, +0.5 },
		{ +0.5, +0.5, -0.5 },
	};

	mesh.normals = {
		{ 0.0, 0.0, -1.0 },
		{ 0.0, 0.0, -1.0 },
		{ 0.0, 0.0, -1.0 },
		{ 0.0, 0.0, -1.0 },
		
		{ 0.0, 0.0, +1.0 },
		{ 0.0, 0.0, +1.0 },
		{ 0.0, 0.0, +1.0 },
		{ 0.0, 0.0, +1.0 },
		
		{ -1.0, 0.0, 0.0 },
		{ -1.0, 0.0, 0.0 },
		{ -1.0, 0.0, 0.0 },
		{ -1.0, 0.0, 0.0 },
		
		{ +1.0, 0.0, 0.0 },
		{ +1.0, 0.0, 0.0 },
		{ +1.0, 0.0, 0.0 },
		{ +1.0, 0.0, 0.0 },
		
		{ 0.0, -1.0, 0.0 },
		{ 0.0, -1.0, 0.0 },
		{ 0.0, -1.0, 0.0 },
		{ 0.0, -1.0, 0.0 },
		
		{ 0.0, +1.0, 0.0 },
		{ 0.0, +1.0, 0.0 },
		{ 0.0, +1.0, 0.0 },
		{ 0.0, +1.0, 0.0 },		
	};

	mesh.indices = {
		0 + 4*0, 1 + 4*0, 2 + 4*0, 2 + 4*0, 3 + 4*0, 0 + 4*0,
		0 + 4*1, 1 + 4*1, 2 + 4*1, 2 + 4*1, 3 + 4*1, 0 + 4*1,
		0 + 4*2, 1 + 4*2, 2 + 4*2, 2 + 4*2, 3 + 4*2, 0 + 4*2,
		0 + 4*3, 1 + 4*3, 2 + 4*3, 2 + 4*3, 3 + 4*3, 0 + 4*3,
		0 + 4*4, 1 + 4*4, 2 + 4*4, 2 + 4*4, 3 + 4*4, 0 + 4*4,
		0 + 4*5, 1 + 4*5, 2 + 4*5, 2 + 4*5, 3 + 4*5, 0 + 4*5,
	};

	return mesh;
}

static ti::Component::Mesh PlaneModel() {
	ti::Component::Mesh mesh;

	mesh.primitive = ti::Primitive::TRIANGLE;

	mesh.positions = {
		{ -0.5, 0.0, -0.5 },
		{ 0.5, 0.0, -0.5 },
		{ 0.5, 0.0, 0.5 },
		{ 0.5, 0.0, 0.5 },
		{ -0.5, 0.0, 0.5 },
		{ -0.5, 0.0, -0.5 },
	};

	mesh.normals = {
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
	};

	return mesh;
}

ti::MeshRegistry::MeshRegistry() {
	AddMesh("Plane", PlaneModel());
	AddMesh("Cube2", CubeModel());
}

bool ti::MeshRegistry::Contains(std::string name) {
	return (registry.find(name) != registry.end());
}

ti::Component::Mesh& ti::MeshRegistry::GetMesh(std::string name) {
	return registry[name];
}

void ti::MeshRegistry::AddMesh(std::string name, ti::Component::Mesh mesh) {
	registry[name] = mesh;
}
