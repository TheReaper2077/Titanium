#include "Renderer.h"

void ti::Renderer::Init(SDL_Window *window) {
	uniformbuffer = UniformBuffer_Create();
	uniformbuffer->Allocate(sizeof(glm::mat4) * 3);
	uniformbuffer->BindRange(0, sizeof(glm::mat4) * 3);
	this->window = window;

	drawbuffer.vertexarray = Vertex::GetVertexArray();
	drawbuffer.indexbuffer = IndexBuffer_Create(Vertex::GetVertexArray());
	drawbuffer.vertexbuffer = VertexBuffer_Create();
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
	shader->Bind();

	shader->SetUniformVec3("material.ambient", &mesh->material.ambient[0]);
	shader->SetUniformVec3("material.diffuse", &mesh->material.diffuse[0]);
	shader->SetUniformVec3("material.specular", &mesh->material.specular[0]);
	shader->SetUniformf("material.shininess", mesh->material.shininess);

	shader->UnBind();

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

void ti::Renderer::RenderPreset() {
	drawbuffer.vertexarray->Bind();
	drawbuffer.vertexarray->BindVertexBuffer(drawbuffer.vertexbuffer, drawbuffer.vertexarray->stride);

	if (drawbuffer.primitive == QUAD) {
		drawbuffer.vertexarray->BindIndexBuffer(drawbuffer.indexbuffer);

		glDrawElements(GL_TRIANGLES, drawbuffer.vertices.size() * 1.5, GL_UNSIGNED_INT, nullptr);
	}
}


