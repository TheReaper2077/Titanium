#pragma once

#include "Components/Components.h"
#include <unordered_map>

namespace ti {
	struct MeshRegistry {
		std::unordered_map<std::string, ti::Component::Mesh> registry;

		MeshRegistry();

		bool Contains(std::string name);
		
		ti::Component::Mesh& GetMesh(std::string name);

		void AddMesh(std::string name, ti::Component::Mesh mesh);
	};
}