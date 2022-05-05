#pragma once

#include "ECS.h"


namespace ti {
	void Serialize(ECS::Registry* registry, const char* filename);

	void DeSerialize(ECS::Registry* registry, const char* filename);
}