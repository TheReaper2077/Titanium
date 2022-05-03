#pragma once

#include "Components/Components.h"
#include <unordered_map>
#include "assert.h"

namespace ti {
	struct MeshRegistry {
		std::unordered_map<std::string, ti::Component::Mesh> registry;

		bool Contains(std::string name) {
			return (registry.find(name) != registry.end());
		}
		
		ti::Component::Mesh& GetMesh(std::string name) {
			return registry[name];
		}

		void AddMesh(std::string name, ti::Component::Mesh mesh) {
			registry[name] = mesh;
		}
	};
}