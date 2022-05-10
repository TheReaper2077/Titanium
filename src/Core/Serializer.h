#pragma once

#include "ECS.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/emitter.h"

#include <glm/glm.hpp>

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::bvec3& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v);

template<> struct YAML::convert<glm::vec2>;
template<> struct YAML::convert<glm::vec3>;
template<> struct YAML::convert<glm::bvec3>;
template<> struct YAML::convert<glm::vec4>;

namespace ti {	
	void SerializeEntities(ECS::Registry* registry, const char* filename);
	void DeSerializeEntities(ECS::Registry* registry, const char* filename);
	void SerializeMaterials(ECS::Registry* registry, const char* filename);
	void DeSerializeMaterials(ECS::Registry* registry, const char* filename);
}