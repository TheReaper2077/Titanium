#include "Game.h"
#include "Camera.h"
#include "../Core/Components/Components.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

ti::Camera* camera;

UniformBuffer *uniformbuffer;
Shader* shader;

Assimp::Importer importer;

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct Mesh {
	std::vector<ti::Vertex> vertices;
	std::vector<uint32_t> indices;
    std::vector<Texture2D*> textures;
	
	VertexArray* vertexarray;
	VertexBuffer* vertexbuffer;
	IndexBuffer* indexbuffer;

	Material material;
};

std::vector<Mesh> meshes;

Mesh process_mesh(aiMesh *ai_mesh, const aiScene *scene) {
	Mesh mesh;

	for (int i = 0; i < ai_mesh->mNumVertices; i++) {
		ti::Vertex vertex;

		vertex.pos.x = ai_mesh->mVertices[i].x;
		vertex.pos.y = ai_mesh->mVertices[i].y;
		vertex.pos.z = ai_mesh->mVertices[i].z;

		vertex.normal.x = ai_mesh->mNormals[i].x;
		vertex.normal.y = ai_mesh->mNormals[i].y;
		vertex.normal.z = ai_mesh->mNormals[i].z;

		if (ai_mesh->mTextureCoords[0]) {
			vertex.attr.x = ai_mesh->mTextureCoords[0][i].x;
			vertex.attr.y = ai_mesh->mTextureCoords[0][i].y;
			vertex.attr.z = 0;
		}

		mesh.vertices.push_back(vertex);
	}

	for (int i = 0; i < ai_mesh->mNumFaces; i++) {
		aiFace face = ai_mesh->mFaces[i];

		for(unsigned int j = 0; j < face.mNumIndices; j++) {
			mesh.indices.push_back(face.mIndices[j]);
		}
	}

	if (ai_mesh->mMaterialIndex >= 0) {
		aiMaterial* ai_material = scene->mMaterials[ai_mesh->mMaterialIndex];
		aiString str;

		for (int i = 0; i < ai_material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
			ai_material->GetTexture(aiTextureType_DIFFUSE, i, &str);

			mesh.textures.push_back(Texture_LoadFile(str.C_Str()));
		}
		
		// for (int)
	}

	return mesh;
}

void process_node(aiNode *node, const aiScene *scene) {
	for (int i = 0; i < node->mNumMeshes; i++) {
		auto* mesh = scene->mMeshes[i];
		meshes.push_back(process_mesh(mesh, scene));
	}

	for (int i = 0; i < node->mNumChildren; i++) {
		process_node(node->mChildren[i], scene);
	}
}

void Game::Init() {
	camera = new ti::Camera("cam0", ti::ProjectionMode::PERSPECTIVE, width, height);
	renderer->SetCamera(camera);

	auto player = registry->Create();
	registry->Add<ti::Component::Properties>(player, "player", player);
	registry->Add<ti::Component::Transform>(player);
	
	uniformbuffer = UniformBuffer_Create();
	uniformbuffer->Allocate(sizeof(glm::mat4) * 3);
	uniformbuffer->BindRange(0, sizeof(glm::mat4) * 3);

	shader = Shader_Create("color", "D:\\C++\\2.5D Engine\\src\\Shaders\\default.vs", "D:\\C++\\2.5D Engine\\src\\Shaders\\color.fs");

	const aiScene *scene = importer.ReadFile("D:\\C++\\2.5D Engine\\res\\cylinder.obj", aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
	assert(scene && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode);

	process_node(scene->mRootNode, scene);

	for (auto& mesh: meshes) {
		mesh.vertexarray = ti::Vertex::GetVertexArray();
		mesh.vertexbuffer = VertexBuffer_Create();
		mesh.indexbuffer = IndexBuffer_Create(mesh.vertexarray);

		mesh.vertexbuffer->AddDataStatic(mesh.vertices.data(), sizeof(ti::Vertex)*mesh.vertices.size());
		mesh.indexbuffer->AddData(mesh.indices.data(), sizeof(uint32_t)*mesh.indices.size());
	}

	SDL_ShowCursor(SDL_DISABLE);
}

void Game::Update(double dt) {
	glViewport(0, 0, this->width, this->height);
	glClearColor(0.2, 0.2, 0.2, 0.2);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// if (eventdata->mouseposx > width)
	// 	eventdata->mouseposx = 1;
	// if (eventdata->mouseposx <= 0)
	// 	eventdata->mouseposx = width;

	// if (eventdata->mouseposy > height)
	// 	eventdata->mouseposy = 1;
	// if (eventdata->mouseposy <= 0)
	// 	eventdata->mouseposy = height;

	camera->Update(dt, eventdata);
		
	uniformbuffer->AddDataDynamic(&camera->model[0][0], sizeof(glm::mat4), sizeof(glm::mat4) * 0);
	uniformbuffer->AddDataDynamic(&camera->view[0][0], sizeof(glm::mat4), sizeof(glm::mat4) * 1);
	uniformbuffer->AddDataDynamic(&camera->projection[0][0], sizeof(glm::mat4), sizeof(glm::mat4) * 2);

	shader->Bind();

	for (auto& mesh: meshes) {
		mesh.vertexarray->Bind();
		mesh.vertexarray->BindVertexBuffer(mesh.vertexbuffer, mesh.vertexarray->stride);
		mesh.vertexarray->BindIndexBuffer(mesh.indexbuffer);

		shader->SetUniformi("", 0);

		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
	}
}
