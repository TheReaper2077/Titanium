#pragma once

#include "ECS.h"


namespace ti {
	void SerializeEntities(ECS::Registry* registry, const char* filename);
	void DeSerializeEntities(ECS::Registry* registry, const char* filename);
}