#pragma once

#include <glm/mat4x4.hpp>
#include <unordered_map>
#include <string>

#include "Mesh.h"
#include "Camera.h"
#include "Transform.h"
#include <OpenGL.h>

namespace ti {
	// struct DrawCmd {
	// 	uint32_t index_first = 0;
	// 	uint32_t index_last = 0;
	// 	uint32_t vertex_first = 0;
	// 	uint32_t vertex_last = 0;

	// 	void Clear() {
	// 		index_first = 0;
	// 		index_last = 0;
	// 		vertex_first = 0;
	// 		vertex_last = 0;
	// 	}
	// };

	struct DrawBuffer {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		VertexArray* vertexarray;
		IndexBuffer* indexbuffer;
		VertexBuffer* vertexbuffer;

		uint32_t indexcount;

		Primitive primitive;

		void Clear() {
			vertices.clear();
			indices.clear();
		}
	};

	class Renderer {
	public:
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 model;

		Vec3 color = Vec3(1, 0, 1);

		std::unordered_map<std::string, Shader*> shader_map;
		std::unordered_map<std::string, Camera*> camera_map;

		UniformBuffer* uniformbuffer;

		SDL_Window* window;
		
		Transform transform;
		Material material;

		DrawBuffer drawbuffer;

		Shader* shader = nullptr;
		Camera* camera = nullptr;
		// DrawCmd drawcmd;

		void Init(SDL_Window* window);
		void SetModel(const glm::mat4& model);
		void SetView(const glm::mat4& view);
		void SetProjection(const glm::mat4& projection);
		void RenderMesh(Mesh* mesh, Shader* shader);
		
		void AddCamera(Camera* camera);
		Camera* GetCamera(std::string name);
		void SetCamera(Camera* camera);
		void SetCamera(std::string name);
		void AddShader(Shader* shader);
		Shader* GetShader(std::string name);
		void SetShader(Shader* shader);
		void SetShader(std::string name);
		void SetTransform(Transform tranform);
		void SetMaterial(Material material);

		void RenderPreset();

		void DrawRect(float x, float y, float w, float h) {
			// auto& renderer = GetEngine()->renderer;
			// auto& vertices = GetEngine()->renderer.drawbuffer.vertices;
			// auto& indices = GetEngine()->renderer.drawbuffer.indices;
			// auto& color = GetEngine()->renderer.color;
			// auto& primitive = GetEngine()->renderer.drawbuffer.primitive;

			if (drawbuffer.primitive != QUAD) {
				RenderPreset();
				drawbuffer.primitive = QUAD;
			}

			// if (shader == nullptr)
			// 	SetShader("color");

			// if (shader->name != std::string("color")) {
			// 	RenderPreset();
			// 	SetShader("color");
			// }

			drawbuffer.vertices.reserve(4 + drawbuffer.vertices.size());

			auto A = Vec3(x, y, 0);
			auto B = Vec3(x + w, y, 0);
			auto C = Vec3(x + w, y + h, 0);

			auto normal = (B - A).cross(C - A).normalize();

			drawbuffer.vertices.emplace_back(Vertex{A, color, normal});
			drawbuffer.vertices.emplace_back(Vertex{B, color, normal});
			drawbuffer.vertices.emplace_back(Vertex{C, color, normal});
			drawbuffer.vertices.emplace_back(Vertex{Vec3(x, y + h, 0), color, normal});

			auto idx = drawbuffer.indices.size();
			drawbuffer.indices.reserve(6 + drawbuffer.indices.size());

			drawbuffer.indices.emplace_back(idx + 0);
			drawbuffer.indices.emplace_back(idx + 1);
			drawbuffer.indices.emplace_back(idx + 2);
			drawbuffer.indices.emplace_back(idx + 2);
			drawbuffer.indices.emplace_back(idx + 3);
			drawbuffer.indices.emplace_back(idx + 0);
		}

		void TexRect(int x, int y, int w, int h, const Vec2& uv0, const Vec2& uv1) {
			// auto& renderer = GetEngine()->renderer;
			// auto& vertices = GetEngine()->renderer.drawbuffer.vertices;
			// auto& indices = GetEngine()->renderer.drawbuffer.indices;
			// auto& color = GetEngine()->renderer.color;
			// auto& primitive = GetEngine()->renderer.drawbuffer.primitive;

			if (drawbuffer.primitive != QUAD) {
				RenderPreset();
				drawbuffer.primitive = QUAD;
			}

			// if (shader == nullptr)
			// 	SetShader("texture");

			// if (shader->name != std::string("texture")) {
			// 	RenderPreset();
			// 	SetShader("texture");
			// }

			auto A = Vec3(x, y, 0);
			auto B = Vec3(x + w, y, 0);
			auto C = Vec3(x + w, y + h, 0);

			auto normal = (B - A).cross(C - A).normalize();

			drawbuffer.vertices.reserve(4 + drawbuffer.vertices.size());

			drawbuffer.vertices.emplace_back(Vertex{A, Vec3(uv0.x, uv0.y, 0), normal});
			drawbuffer.vertices.emplace_back(Vertex{B, Vec3(uv1.x, uv0.y, 0), normal});
			drawbuffer.vertices.emplace_back(Vertex{C, Vec3(uv1.x, uv1.y, 0), normal});
			drawbuffer.vertices.emplace_back(Vertex{Vec3(x, y + h, 0), Vec3(uv0.x, uv1.y, 0), normal});

			auto idx = drawbuffer.indices.size();
			drawbuffer.indices.reserve(6 + drawbuffer.indices.size());

			drawbuffer.indices.emplace_back(idx + 0);
			drawbuffer.indices.emplace_back(idx + 1);
			drawbuffer.indices.emplace_back(idx + 2);
			drawbuffer.indices.emplace_back(idx + 2);
			drawbuffer.indices.emplace_back(idx + 3);
			drawbuffer.indices.emplace_back(idx + 0);
		}
	};
}