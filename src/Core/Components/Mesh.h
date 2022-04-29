#pragma once

#include <glm/vec3.hpp>

#include <OpenGL.h>
#include <vector>
#include "Material.h"
#include "../Enumerations.h"

namespace ti {
	struct Vertex {
		glm::vec3 position;
		glm::vec3 uv;
		glm::vec3 normal;
	};

	namespace Component {
		struct Mesh {
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			
			unsigned int vertexcount = 0;
			unsigned int indexcount = 0;

			VertexBuffer* vertexbuffer = nullptr;
			VertexArray* vertexarray = nullptr;
			IndexBuffer* indexbuffer = nullptr;

			std::string material;
			Primitive primitive = TRIANGLE;

			bool changed = true;

			Mesh() {}
			Mesh(const aiScene* ai_scene, aiMesh* ai_mesh) {
				for (int i = 0; i < ai_mesh->mNumVertices; i++) {
					Vertex vertex;

					vertex.position.x = ai_mesh->mVertices[i].x;
					vertex.position.y = ai_mesh->mVertices[i].y;
					vertex.position.z = ai_mesh->mVertices[i].z;

					vertex.normal.x = ai_mesh->mNormals[i].x;
					vertex.normal.y = ai_mesh->mNormals[i].y;
					vertex.normal.z = ai_mesh->mNormals[i].z;

					if (ai_mesh->mTextureCoords[0]) {
						vertex.uv.x = ai_mesh->mTextureCoords[0][i].x;
						vertex.uv.y = ai_mesh->mTextureCoords[0][i].y;
						vertex.uv.z = 0;
					}

					vertices.push_back(vertex);
				}

				for (int i = 0; i < ai_mesh->mNumFaces; i++) {
					aiFace face = ai_mesh->mFaces[i];

					for(unsigned int j = 0; j < face.mNumIndices; j++) {
						indices.push_back(face.mIndices[j]);
					}
				}

				if (ai_mesh->mMaterialIndex >= 0) {
					material = std::string(ai_scene->mMaterials[ai_mesh->mMaterialIndex]->GetName().C_Str());
				}
			}
			
			void Clear();
		};
	}
}