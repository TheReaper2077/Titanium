#include "Mesh.h"

ti::Mesh::Mesh(Primitive primitive, Material material): material(material), primitive(primitive) {
	if (primitive == QUAD) {
		indexbuffer = IndexBuffer_Create(vertexarray);
		indexed = true;
	}

	vertexbuffer = VertexBuffer_Create();
	vertexarray = Vertex::GetVertexArray();
}

void ti::Mesh::RecalculateNormals() {
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

void ti::Mesh::Transfer() {
	vertexcount = vertices.size();
	vertexbuffer->AddDataStatic(vertices.data(), sizeof(Vertex) * vertexcount);

	if (!indexed || primitive != QUAD) return;

	indexcount = indices.size();
	indexbuffer->AddData(indices.data(), sizeof(uint32_t) * indexcount);
}

void ti::Mesh::Clear() {
	indices.clear();
	vertices.clear();
}
