#pragma once

#include <unordered_map>
#include <iostream>

#include "System.h"
#include "../Functions.h"
#include "../MaterialRegistry.h"
#include "../MeshRegistry.h"
#include "../SpriteRendererRegistry.h"
#include "../ShaderRegistry.h"
#include "../WindowContext.h"

namespace ti {
	namespace System {
		class Renderer {
			UniformBuffer* uniformbuffer = nullptr;

			ti::ECS::Registry* registry;

			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 projection;
			glm::vec3 view_position;

			Shader* shader;
			std::unordered_map<uint32_t, VertexArray*> vertexarray_registry;
		
		public:
			Renderer() {}

			void Init(ti::ECS::Registry *registry);

			VertexArray* GetVertexArray(uint32_t flags);

			void SetShader(Shader* shader);
			void SetMaterial(std::string name);
			void SetMaterial(ti::Component::Material& material);
			
			void SetModel(const glm::mat4& model);
			void SetView(const glm::mat4& view);
			void SetProjection(const glm::mat4& projection);
			void SetViewPosition(const glm::vec3& view_position);
			
			IndexBuffer* TransferQuadIndices(VertexArray* vertexarray, int vertexcount);
			void TransferMesh(ti::Component::Mesh& mesh, ti::Component::MeshRenderer& meshrenderer);

			void Render(Primitive primitive, std::string material, VertexArray* vertexarray, int vertexcount, VertexBuffer* vertexbuffer, int indexcount = 0, IndexBuffer* indexbuffer = nullptr);
			void RenderSprite(int vertexcount);
			void RenderColliders();
			void RenderCubeWire(const glm::vec3& center, const glm::vec3& size);

			void Render(WindowContext& window);
		};
	}
}
