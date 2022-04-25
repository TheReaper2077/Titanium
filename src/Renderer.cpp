#include "Renderer.h"

void ti::Renderer::Init(SDL_Window *window) {
	uniformbuffer = UniformBuffer_Create();
	uniformbuffer->Allocate(sizeof(glm::mat4) * 3);
	uniformbuffer->BindRange(0, sizeof(glm::mat4) * 3);
	this->window = window;

	drawbuffer.vertexarray = VertexArray_Create({{ 0, 3, GL_FLOAT }, { 1, 3, GL_FLOAT }, { 2, 3, GL_FLOAT }});
	drawbuffer.indexbuffer = IndexBuffer_Create(drawbuffer.vertexarray);
	drawbuffer.vertexbuffer = VertexBuffer_Create();

	auto* a = Shader_Create("color", "D:\\C++\\2.5D Engine\\src\\Shaders\\default.vs", "D:\\C++\\2.5D Engine\\src\\Shaders\\color.vs");
	a->Bind();

	AddShader("color", a);
	AddShader("texture", Shader_Create("texture", "D:\\C++\\2.5D Engine\\src\\Shaders\\default.vs", "D:\\C++\\2.5D Engine\\src\\Shaders\\texture.vs"));
	AddShader("sprite", Shader_Create("sprite", "D:\\C++\\2.5D Engine\\src\\Shaders\\default.vs", "D:\\C++\\2.5D Engine\\src\\Shaders\\sprite.vs"));
}

Shader* ti::Renderer::AddShader(std::string name, Shader* shader) {
	shader_map[name] = shader;
	return shader_map[name];
}

Shader* ti::Renderer::GetShader(std::string name) {
	return shader_map[name];
}

void ti::Renderer::SetModel(const glm::mat4& model) {
	if (this->model == model) return;
	this->model = model;
	uniformbuffer->AddDataDynamic(&this->model[0][0], sizeof(glm::mat4), sizeof(glm::mat4) * 0);
}

void ti::Renderer::SetView(const glm::mat4& view) {
	if (this->view == view) return;
	this->view = view;
	uniformbuffer->AddDataDynamic(&this->view[0][0], sizeof(glm::mat4), sizeof(glm::mat4) * 1);
}

void ti::Renderer::SetProjection(const glm::mat4& projection) {
	if (this->projection == projection) return;
	this->projection = projection;
	uniformbuffer->AddDataDynamic(&this->projection[0][0], sizeof(glm::mat4), sizeof(glm::mat4) * 2);
}

void ti::Renderer::RenderMesh(Mesh* mesh, Shader* shader) {
	SetShader(shader);
	SetMaterial(mesh->material);

	shader->Bind();
	mesh->vertexarray->Bind();
	mesh->vertexarray->BindVertexBuffer(mesh->vertexbuffer, mesh->vertexarray->stride);

	if (mesh->indexed || mesh->primitive == QUAD) {
		mesh->vertexarray->BindIndexBuffer(mesh->indexbuffer);
	}

	if (mesh->primitive == LINE)
		glDrawArrays(GL_LINES, 0, mesh->vertexcount);
	if (mesh->primitive == POINT)
		glDrawArrays(GL_POINTS, 0, mesh->vertexcount);
	if (mesh->primitive == TRIANGLE)
		glDrawArrays(GL_TRIANGLES, 0, mesh->vertexcount);
	if (mesh->primitive == LINE && mesh->indexed)
		glDrawElements(GL_LINES, mesh->indexcount, GL_UNSIGNED_INT, nullptr);
	if (mesh->primitive == POINT && mesh->indexed)
		glDrawElements(GL_POINTS, mesh->indexcount, GL_UNSIGNED_INT, nullptr);
	if (mesh->primitive == TRIANGLE && mesh->indexed)
		glDrawElements(GL_TRIANGLES, mesh->indexcount, GL_UNSIGNED_INT, nullptr);
	if (mesh->primitive == QUAD)
		glDrawElements(GL_TRIANGLES, mesh->vertexcount * 1.5, GL_UNSIGNED_INT, nullptr);
}

ti::Camera* ti::Renderer::AddCamera(Camera* camera) {
	camera_map[camera->name] = camera;
	return camera_map[camera->name];
}

ti::Camera* ti::Renderer::GetCamera(std::string name) {
	return camera_map[name];
}

void ti::Renderer::SetCamera(Camera* camera) {
	this->camera = camera;

	SetProjection(camera->projection);
	SetView(camera->view);
	SetModel(camera->model);
}

void ti::Renderer::SetCamera(std::string name) {
	SetCamera(GetCamera(name));
}

void ti::Renderer::SetShader(Shader* shader) {
	this->shader = shader;
}

void ti::Renderer::SetShader(std::string name) {
	SetShader(GetShader(name));
}

void ti::Renderer::SetTransform(Transform tranform) {
	this->transform = transform;

	SetModel(transform.GetModel());
}

void ti::Renderer::SetMaterial(Material material) {
	this->material = material;

	shader->Bind();

	shader->SetUniformVec3("material.ambient", &material.ambient[0]);
	shader->SetUniformVec3("material.diffuse", &material.diffuse[0]);
	shader->SetUniformVec3("material.specular", &material.specular[0]);
	shader->SetUniformf("material.shininess", material.shininess);
}

void ti::Renderer::RenderPreset() {
	if (!drawbuffer.vertices.size()) return;

	shader->Bind();

	drawbuffer.vertexarray->Bind();
	drawbuffer.vertexarray->BindVertexBuffer(drawbuffer.vertexbuffer, drawbuffer.vertexarray->stride);
	// drawbuffer.vertexbuffer->Allocate(drawbuffer.vertices.size()*sizeof(Vertex));
	// drawbuffer.vertexbuffer->AddDataDynamic(drawbuffer.vertices.data(), drawbuffer.vertices.size()*sizeof(Vertex));
	drawbuffer.vertexbuffer->AddDataStatic(drawbuffer.vertices.data(), drawbuffer.vertices.size()*sizeof(Vertex));

	if (drawbuffer.primitive == QUAD) {
		if (!drawbuffer.indices.size()) return;

		drawbuffer.indexbuffer->AddData(drawbuffer.indices.data(), drawbuffer.indices.size()*sizeof(uint32_t));
		drawbuffer.vertexarray->BindIndexBuffer(drawbuffer.indexbuffer);

		glDrawElements(GL_TRIANGLES, drawbuffer.vertices.size() * 1.5, GL_UNSIGNED_INT, nullptr);
	}
	
	drawbuffer.vertices.clear();
	drawbuffer.indices.clear();

	// std::cout << 
}
