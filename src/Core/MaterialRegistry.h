#pragma once

#include <unordered_map>
#include <string>
#include "Components/Components.h"
#include "uuid.h"

namespace ti {
	struct MaterialRegistry {
		std::unordered_map<std::string, ti::Component::Material> registry;
	
		void RegisterMaterial(const ti::Component::Material& material) {
			// assert(registry.find(material.name) == registry.end());

			registry[material.name] = material;
		}

		ti::Component::Material& GetMaterial(std::string name) {
			assert(registry.find(name) != registry.end());

			return registry[name];
		}

		bool Contains(std::string material) {
			return (registry.find(material) != registry.end());
		}
	};
}