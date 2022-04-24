#pragma once

#include <glm/mat4x4.hpp>
#include <unordered_map>
#include <string>

#include "Mesh.h"
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

		Vec3 color;

		std::unordered_map<std::string, Shader*> shader_map;

		UniformBuffer* uniformbuffer;

		uint32_t next_id = 0;

		SDL_Window* window;
		
		Transform transform;

		DrawBuffer drawbuffer;
		// DrawCmd drawcmd;

		void Init(SDL_Window* window);
		Shader* AddShader(std::string name, Shader* shader);
		Shader* GetShader(std::string name);
		void SetModel(const glm::mat4& model);
		void SetView(const glm::mat4& view);
		void SetProjection(const glm::mat4& projection);
		void RenderMesh(Mesh* mesh, Shader* shader);
		
		void SetTransform(const Transform& tranform);

		void RenderPreset();

		void DrawRect(int x, int y, int w, int h) {
			// auto& renderer = GetEngine()->renderer;
			// auto& vertices = GetEngine()->renderer.drawbuffer.vertices;
			// auto& indices = GetEngine()->renderer.drawbuffer.indices;
			// auto& color = GetEngine()->renderer.color;
			// auto& primitive = GetEngine()->renderer.drawbuffer.primitive;

			if (drawbuffer.primitive != QUAD) {
				RenderPreset();
				drawbuffer.primitive = QUAD;
			}

			drawbuffer.vertices.reserve(4 + drawbuffer.vertices.size());

			drawbuffer.vertices.emplace_back(Vertex{Vec3(x, y, 0), color, Vec3()});
			drawbuffer.vertices.emplace_back(Vertex{Vec3(x + w, y, 0), color, Vec3()});
			drawbuffer.vertices.emplace_back(Vertex{Vec3(x + w, y + h, 0), color, Vec3()});
			drawbuffer.vertices.emplace_back(Vertex{Vec3(x, y + h, 0), color, Vec3()});

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

			drawbuffer.vertices.reserve(4 + drawbuffer.vertices.size());

			drawbuffer.vertices.emplace_back(Vertex{Vec3(uv0.x, uv0.y, 0), color, Vec3()});
			drawbuffer.vertices.emplace_back(Vertex{Vec3(uv1.x, uv0.y, 0), color, Vec3()});
			drawbuffer.vertices.emplace_back(Vertex{Vec3(uv1.x, uv1.y, 0), color, Vec3()});
			drawbuffer.vertices.emplace_back(Vertex{Vec3(uv0.x, uv1.y, 0), color, Vec3()});

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