#pragma once

#include "define.h"
#include "OpenGL/OpenGL.h"
#include "Vertex.h"
#include "Material.h"

namespace ti {
	enum Primitive {
		POINT,
		LINE,
		TRIANGLE,
		QUAD,
	};

	struct Mesh {
		VertexArray* vertexarray;
		VertexBuffer* vertexbuffer;
		IndexBuffer* indexbuffer;

		Primitive primitive;
		bool indexed = false;

		uint32_t vertexcount;
		std::vector<Vertex> vertices;
		uint32_t indexcount;
		std::vector<uint32_t> indices;

		// std::unordered_map<Texture*, uint32_t> texture_map;

		Material material;

		Mesh(Primitive primitive, Material material, bool indexed = false): material(material), primitive(primitive), indexed(indexed) {
			if (indexed || primitive == QUAD) {
				indexbuffer = IndexBuffer_Create(vertexarray);
			}

			vertexbuffer = VertexBuffer_Create();
			vertexarray = Vertex::GetVertexArray();
		}

		void RecalculateNormals() {
			if (primitive == TRIANGLE) {
				for (int i = 0; i < vertices.size() / 3; i++) {
					auto A = vertices[i*3 + 0].pos;
					auto B = vertices[i*3 + 1].pos;
					auto C = vertices[i*3 + 2].pos;

					auto normal = (B - A).cross(C - A).normalize();

					vertices[i*3 + 0].normal = normal;
					vertices[i*3 + 1].normal = normal;
					vertices[i*3 + 2].normal = normal;
				}
			}
			if (primitive == QUAD) {
				for (int i = 0; i < vertices.size() / 4; i++) {
					auto A = vertices[i*4 + 0].pos;
					auto B = vertices[i*4 + 1].pos;
					auto C = vertices[i*4 + 2].pos;

					auto normal = (B - A).cross(C - A).normalize();

					vertices[i*4 + 0].normal = normal;
					vertices[i*4 + 1].normal = normal;
					vertices[i*4 + 2].normal = normal;
					vertices[i*4 + 3].normal = normal;
				}
			}
		}

		void Transfer() {
			vertexcount = vertices.size();
			vertexbuffer->AddDataStatic(vertices.data(), sizeof(Vertex) * vertexcount);

			if (!indexed || primitive != QUAD) return;

			indexcount = indices.size();
			indexbuffer->AddData(indices.data(), sizeof(uint32_t) * indexcount);
		}

		void Clear() {
			indices.clear();
			vertices.clear();
		}
	};
}