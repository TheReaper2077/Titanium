#pragma once

#include "Enumerations.h"
#include "MeshRegistry.h"
#include "Components/Components.h"
#include "assert.h"

static ti::Component::Mesh CubeModel() {
	ti::Component::Mesh mesh;

	mesh.primitive = ti::Primitive::TRIANGLE;

	mesh.positions = {
		{ 0.5, 0.5, -0.5 },
		{ -0.5, 0.5, -0.5 },
		{ -0.5, 0.5, 0.5 },
		{ 0.5, 0.5, 0.5 },
		{ 0.5, -0.5, 0.5 },
		{ 0.5, 0.5, 0.5 },
		{ -0.5, 0.5, 0.5 },
		{ -0.5, -0.5, 0.5 },
		{ -0.5, -0.5, 0.5 },
		{ -0.5, 0.5, 0.5 },
		{ -0.5, 0.5, -0.5 },
		{ -0.5, -0.5, -0.5 },
		{ -0.5, -0.5, -0.5 },
		{ 0.5, -0.5, -0.5 },
		{ 0.5, -0.5, 0.5 },
		{ -0.5, -0.5, 0.5 },
		{ 0.5, -0.5, -0.5 },
		{ 0.5, 0.5, -0.5 },
		{ 0.5, 0.5, 0.5 },
		{ 0.5, -0.5, 0.5 },
		{ -0.5, -0.5, -0.5 },
		{ -0.5, 0.5, -0.5 },
		{ 0.5, 0.5, -0.5 },
		{ 0.5, -0.5, -0.5 },
	};

	mesh.normals = {
		{ 0.0000, 1.0000, 0.0000 },
		{ 0.0000, 0.0000, 1.0000 },
		{ -1.0000, 0.0000, 0.0000 },
		{ 0.0000, -1.0000, 0.0000 },
		{ 1.0000, 0.0000, 0.0000 },
		{ 0.0000, 0.0000, -1.0000 },
	};

	mesh.indices = {
		
	};

	return mesh;
}

static ti::Component::Mesh PlaneModel() {
	ti::Component::Mesh mesh;

	mesh.primitive = ti::Primitive::TRIANGLE;

	mesh.positions = {
		{ 0.0, 0.0, 0.0 },
		{ 0.5, 0.0, 0.0 },
		{ 0.5, 0.0, 0.5 },
		{ 0.5, 0.0, 0.5 },
		{ 0.0, 0.0, 0.5 },
		{ 0.0, 0.0, 0.0 },
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
