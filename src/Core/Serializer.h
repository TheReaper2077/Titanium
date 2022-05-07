#pragma once

#include "ECS.h"


namespace ti {
	void SerializeEntities(ECS::Registry* registry, const char* filename);
	void DeSerializeEntities(ECS::Registry* registry, const char* filename);
	void SerializeMaterials(ECS::Registry* registry, const char* filename);
	void DeSerializeMaterials(ECS::Registry* registry, const char* filename);
}