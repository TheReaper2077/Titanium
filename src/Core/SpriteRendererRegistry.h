#pragma once

#include <OpenGL.h>
#include <unordered_map

namespace ti {
	struct SpriteRendererContext {
		VertexArray* vertexarray = nullptr;
		VertexBuffer* vertexbuffer = nullptr;
		IndexBuffer* indexbuffer = nullptr;
		int indexcount = 0;
		int vertexcount = 0;
		uint32_t flags = 0;

		SpriteRendererContext(uint32_t flags) {
			vertexarray = GetVertexArray(flags);
			vertexbuffer = VertexBuffer_Create();
			indexbuffer = IndexBuffer_Create(vertexarray);
		}
	};

	struct SpriteRendererRegistry {
		std::unordered_map<uint32_t, SpriteRendererContext> registry;

		SpriteRendererContext& GetSpriteRendererContext(uint32_t flags) {
			if (registry.find(flags) == registry.end()) {
				registry[flags] = SpriteRendererContext(flags);
			}

			return registry[flags];
		}
	};
};