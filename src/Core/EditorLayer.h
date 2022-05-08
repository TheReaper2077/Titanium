#pragma once

#include <OpenGL.h>
#include "ECS.h"
// #include "Serializer.h"

// #define DEGREE_TO_RADIANS(x) ((x) * (3.14159265359/180.0))
// #define RADIANS_TO_DEGREE(x) ((x) * (180.0/3.14159265359))

namespace ti {
	class ImGuiLayer {
	public:
		ti::ECS::Registry* registry;

		FrameBuffer* main_fbo = nullptr;

		void Init();

		void BeginMain();

		void EndMain();

		void Inspector(ti::ECS::Entity& entity);

		ti::ECS::Entity Heirarchy();

		void MaterialRegistry();

		void MeshRegistry();

		void Status();

		void Controls();

		void Update();

		void EventHandler();
	};
}
