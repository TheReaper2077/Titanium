#include "RenderSystem.h"

#include "glm/gtx/string_cast.hpp"

void ti::System::RenderSystem::Init(ti::ECS::Registry* registry) {
	this->registry = registry;
}

VertexArray* ti::System::RenderSystem::GetVertexArray(uint32_t flags) {
	if (vertexarray_registry.find(flags) == vertexarray_registry.end()) {
		std::vector<VertexArrayAttribDescriptor> descriptor;

		if (flags & POSITION_ATTRIB_BIT) {
			descriptor.push_back({ position, 0, 3, GL_FLOAT });
		}
		if (flags & NORMAL_ATTRIB_BIT) {
			descriptor.push_back({ normal, 1, 3, GL_FLOAT });
		}
		if (flags & COLOR_ATTRIB_BIT) {
			descriptor.push_back({ color, 2, 4, GL_FLOAT });
		}
		if (flags & UV0_ATTRIB_BIT) {
			descriptor.push_back({ uv0, 3, 2, GL_FLOAT });
		}
		if (flags & UV1_ATTRIB_BIT) {
			descriptor.push_back({ uv1, 4, 2, GL_FLOAT });
		}
		if (flags & UV2_ATTRIB_BIT) {
			descriptor.push_back({ uv2, 5, 2, GL_FLOAT });
		}
		if (flags & UV3_ATTRIB_BIT) {
			descriptor.push_back({ uv3, 6, 2, GL_FLOAT });
		}
		if (flags & UV4_ATTRIB_BIT) {
			descriptor.push_back({ uv4, 7, 2, GL_FLOAT });
		}
		if (flags & UV5_ATTRIB_BIT) {
			descriptor.push_back({ uv5, 8, 2, GL_FLOAT });
		}
		if (flags & UV6_ATTRIB_BIT) {
			descriptor.push_back({ uv6, 9, 2, GL_FLOAT });
		}
		if (flags & UV7_ATTRIB_BIT) {
			descriptor.push_back({ uv7, 10, 2, GL_FLOAT });
		}

		vertexarray_registry[flags] = VertexArray_Create(descriptor);
	}

	return vertexarray_registry[flags];
}
void ti::System::RenderSystem::SetShader(Shader* shader) {
	this->shader = shader;
}
void ti::System::RenderSystem::SetMaterial(std::string name) {
	SetMaterial(registry->Store<MaterialRegistry>().GetMaterial(name));
}
void ti::System::RenderSystem::SetMaterial(ti::Component::Material& material) {
	// shader->SetUniformVec3("material.ambient", &material.ambient[0]);
	// shader->SetUniformVec3("material.diffuse", &material.diffuse[0]);
	// shader->SetUniformVec3("material.specular", &material.specular[0]);

	// shader->SetUniformVec3("material.albedo", &material.albedo[0]);
	// shader->SetUniformf("material.metallic", material.metallic);
	// shader->SetUniformf("material.roughness", material.roughness);
	
	shader->SetUniformVec3("material.albedo", &material.albedo[0]);
	shader->SetUniformf("material.metallic", material.metallic);
	shader->SetUniformf("material.roughness", material.roughness);
	shader->SetUniformf("material.ao", 1.0f);

	// shader->SetUniformf("material.shininess", material.shininess);

	if (material.ambient_map != nullptr) {
		glActiveTexture(GL_TEXTURE0);
		material.ambient_map->BindUnit(0);
		shader->SetUniformi("material.ambient_map", 0);
		shader->SetUniformi("ambient_index", 1);
	} else {
		shader->SetUniformi("ambient_index", 0);
	}

	if (material.diffuse_map != nullptr) {
		glActiveTexture(GL_TEXTURE1);
		material.diffuse_map->BindUnit(1);
		shader->SetUniformi("material.diffuse_map", 1);
		shader->SetUniformi("diffuse_index", 1);
	} else {
		shader->SetUniformi("diffuse_index", 0);
	}

	if (material.specular_map != nullptr) {
		glActiveTexture(GL_TEXTURE2);
		material.specular_map->BindUnit(2);
		shader->SetUniformi("material.specular_map", 2);
		shader->SetUniformi("specular_index", 1);
	} else {
		shader->SetUniformi("specular_index", 0);
	}
}
void ti::System::RenderSystem::SetModel(const glm::mat4& model) {
	if (this->model == model) return;
	this->model = model;
	uniformbuffer->AddDataDynamic(&this->model[0][0], sizeof(glm::mat4), sizeof(glm::mat4) * 0);
}
void ti::System::RenderSystem::SetView(const glm::mat4& view) {
	if (this->view == view) return;
	this->view = view;
	uniformbuffer->AddDataDynamic(&this->view[0][0], sizeof(glm::mat4), sizeof(glm::mat4) * 1);
}
void ti::System::RenderSystem::SetProjection(const glm::mat4& projection) {
	if (this->projection == projection) return;
	this->projection = projection;
	uniformbuffer->AddDataDynamic(&this->projection[0][0], sizeof(glm::mat4), sizeof(glm::mat4) * 2);
}
void ti::System::RenderSystem::SetViewPosition(const glm::vec3& view_position) {
	if (this->view_position == view_position) return;
	this->view_position = view_position;
	uniformbuffer->AddDataDynamic(&this->view_position[0], sizeof(glm::vec3), sizeof(glm::mat4) * 3);
}

IndexBuffer* ti::System::RenderSystem::TransferQuadIndices(VertexArray* vertexarray, int vertexcount) {
	static int indexcount;
	static IndexBuffer* indexbuffer;

	if (vertexcount * 1.5 > indexcount) {
		if (indexbuffer == nullptr) indexbuffer = IndexBuffer_Create(vertexarray);
		
		uint32_t* indices = (uint32_t*)malloc(sizeof(unsigned int) * vertexcount * 1.5);
		for (int i = 0; i < vertexcount / 4; i += 4) {
			indices[int(i * 1.5 + 0)] = 0 + i;
			indices[int(i * 1.5 + 1)] = 1 + i;
			indices[int(i * 1.5 + 2)] = 3 + i;
			indices[int(i * 1.5 + 3)] = 2 + i;
			indices[int(i * 1.5 + 4)] = 3 + i;
			indices[int(i * 1.5 + 5)] = 0 + i;
		}

		indexcount = vertexcount * 1.5;
		indexbuffer->AddData(indices, sizeof(uint32_t) * vertexcount * 1.5);
	}

	return indexbuffer;
}

void ti::System::RenderSystem::TransferMesh(ti::Component::Mesh& mesh, ti::Component::MeshRenderer& meshrenderer) {
	uint32_t flags = 0;
	if (mesh.positions.size()) flags |= POSITION_ATTRIB_BIT;
	if (mesh.normals.size()) flags |= NORMAL_ATTRIB_BIT;
	if (mesh.color.size()) flags |= COLOR_ATTRIB_BIT;
	if (mesh.uv0.size()) flags |= UV0_ATTRIB_BIT;
	if (mesh.uv1.size()) flags |= UV1_ATTRIB_BIT;
	if (mesh.uv2.size()) flags |= UV2_ATTRIB_BIT;
	if (mesh.uv3.size()) flags |= UV3_ATTRIB_BIT;
	if (mesh.uv4.size()) flags |= UV4_ATTRIB_BIT;
	if (mesh.uv5.size()) flags |= UV5_ATTRIB_BIT;
	if (mesh.uv6.size()) flags |= UV6_ATTRIB_BIT;
	if (mesh.uv7.size()) flags |= UV7_ATTRIB_BIT;

	if (meshrenderer.vertexarray == nullptr)
		meshrenderer.vertexarray = GetVertexArray(meshrenderer.flags);

	if (!mesh.changed && mesh.indices.size() == mesh.indexcount && mesh.positions.size() == mesh.vertexcount && meshrenderer.flags == flags) return;
	mesh.changed = true;
	meshrenderer.flags = flags;

	mesh.indexcount = mesh.indices.size();
	mesh.vertexcount = mesh.positions.size();

	if (mesh.vertexbuffer == nullptr && mesh.vertexcount)
		mesh.vertexbuffer = VertexBuffer_Create();

	if (mesh.indexbuffer == nullptr && mesh.indexcount)
		mesh.indexbuffer = IndexBuffer_Create(meshrenderer.vertexarray);

	auto* data = (float*)malloc(mesh.vertexcount * meshrenderer.vertexarray->stride);

	for (int i = 0; i < mesh.vertexcount; i++) {
		if (meshrenderer.vertexarray->has_position) {
			if (i < mesh.positions.size()) {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->position_offset + 0] = mesh.positions[i].x;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->position_offset + 1] = mesh.positions[i].y;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->position_offset + 2] = mesh.positions[i].z;
			} else {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->position_offset + 0] = 0;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->position_offset + 1] = 0;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->position_offset + 2] = 0;
			}
		}
		if (meshrenderer.vertexarray->has_normal) {
			if (i < mesh.normals.size()) {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->normal_offset + 0] = mesh.normals[i].x;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->normal_offset + 1] = mesh.normals[i].y;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->normal_offset + 2] = mesh.normals[i].z;
			} else {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->normal_offset + 0] = 0;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->normal_offset + 1] = 0;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->normal_offset + 2] = 0;
			}
		}
		if (meshrenderer.vertexarray->has_color) {
			if (i < mesh.color.size()) {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->color_offset + 0] = mesh.color[i].x;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->color_offset + 1] = mesh.color[i].y;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->color_offset + 2] = mesh.color[i].z;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->color_offset + 3] = mesh.color[i].w;
			} else {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->color_offset + 0] = 0;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->color_offset + 1] = 0;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->color_offset + 2] = 0;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->color_offset + 3] = 0;
			}
		}
		if (meshrenderer.vertexarray->has_uv0) {
			if (i < mesh.uv0.size()) {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv0_offset + 0] = mesh.uv0[i].x;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv0_offset + 1] = mesh.uv0[i].y;
			} else {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv0_offset + 0] = 0;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv0_offset + 1] = 0;
			}
		}
		if (meshrenderer.vertexarray->has_uv1) {
			if (i < mesh.uv1.size()) {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv1_offset + 0] = mesh.uv1[i].x;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv1_offset + 1] = mesh.uv1[i].y;
			} else {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv1_offset + 0] = 0;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv1_offset + 1] = 0;
			}
		}
		if (meshrenderer.vertexarray->has_uv2) {
			if (i < mesh.uv2.size()) {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv2_offset + 0] = mesh.uv2[i].x;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv2_offset + 1] = mesh.uv2[i].y;
			} else {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv2_offset + 0] = 0;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv2_offset + 1] = 0;
			}
		}
		if (meshrenderer.vertexarray->has_uv3) {
			if (i < mesh.uv3.size()) {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv3_offset + 0] = mesh.uv3[i].x;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv3_offset + 1] = mesh.uv3[i].y;
			} else {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv3_offset + 0] = 0;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv3_offset + 1] = 0;
			}
		}
		if (meshrenderer.vertexarray->has_uv4) {
			if (i < mesh.uv4.size()) {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv4_offset + 0] = mesh.uv4[i].x;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv4_offset + 1] = mesh.uv4[i].y;
			} else {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv4_offset + 0] = 0;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv4_offset + 1] = 0;
			}
		}
		if (meshrenderer.vertexarray->has_uv5) {
			if (i < mesh.uv5.size()) {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv5_offset + 0] = mesh.uv5[i].x;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv5_offset + 1] = mesh.uv5[i].y;
			} else {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv5_offset + 0] = 0;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv5_offset + 1] = 0;
			}
		}
		if (meshrenderer.vertexarray->has_uv6) {
			if (i < mesh.uv6.size()) {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv6_offset + 0] = mesh.uv6[i].x;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv6_offset + 1] = mesh.uv6[i].y;
			} else {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv6_offset + 0] = 0;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv6_offset + 1] = 0;
			}
		}
		if (meshrenderer.vertexarray->has_uv7) {
			if (i < mesh.uv7.size()) {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv7_offset + 0] = mesh.uv7[i].x;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv7_offset + 1] = mesh.uv7[i].y;
			} else {
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv7_offset + 0] = 0;
				data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv7_offset + 1] = 0;
			}
		}
	}

	mesh.vertexbuffer->Allocate(mesh.vertexcount * meshrenderer.vertexarray->stride);
	mesh.vertexbuffer->AddDataDynamic((void*)data, mesh.vertexcount * meshrenderer.vertexarray->stride);
	// mesh.vertexbuffer->AddDataStatic((void*)data, mesh.vertexcount*meshrenderer.vertexarray->stride);
	mesh.indexbuffer->AddData(mesh.indices.data(), sizeof(uint32_t) * mesh.indexcount);

	mesh.changed = false;
	free(data);
	data = nullptr;
}

glm::vec3 lightPositions[] = {
	glm::vec3(-10.0f,  10.0f, 10.0f),
	glm::vec3( 10.0f,  10.0f, 10.0f),
	glm::vec3(-10.0f, -10.0f, 10.0f),
	glm::vec3( 10.0f, -10.0f, 10.0f),
};
glm::vec3 lightColors[] = {
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f)
};

void ti::System::RenderSystem::Render(Primitive primitive, std::string material, VertexArray* vertexarray, int vertexcount, VertexBuffer* vertexbuffer, int indexcount, IndexBuffer* indexbuffer) {
	auto& engine = registry->Store<EngineProperties>();
	
	SetMaterial(material);

	vertexarray->Bind();
	vertexarray->BindVertexBuffer(vertexbuffer, vertexarray->stride);
	engine.vertexcount += vertexcount;

	if (indexcount) {
		vertexarray->BindIndexBuffer(indexbuffer);
		engine.indexcount += indexcount;

		if (primitive == ti::Primitive::TRIANGLE) glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_INT, nullptr);
		if (primitive == ti::Primitive::TRIANGLE_FAN) glDrawElements(GL_TRIANGLE_FAN, indexcount, GL_UNSIGNED_INT, nullptr);
		if (primitive == ti::Primitive::TRIANGLE_STRIP) glDrawElements(GL_TRIANGLE_STRIP, indexcount, GL_UNSIGNED_INT, nullptr);
		if (primitive == ti::Primitive::LINE) glDrawElements(GL_LINES, indexcount, GL_UNSIGNED_INT, nullptr);
		if (primitive == ti::Primitive::LINE_STRIP) glDrawElements(GL_LINE_STRIP, indexcount, GL_UNSIGNED_INT, nullptr);
		if (primitive == ti::Primitive::POINT) glDrawElements(GL_POINTS, indexcount, GL_UNSIGNED_INT, nullptr);
		engine.drawcalls++;
	} else {
		if (primitive == ti::Primitive::TRIANGLE) glDrawArrays(GL_TRIANGLES, 0, vertexcount);
		if (primitive == ti::Primitive::TRIANGLE_FAN) glDrawArrays(GL_TRIANGLE_FAN, 0, vertexcount);
		if (primitive == ti::Primitive::TRIANGLE_STRIP) glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexcount);
		if (primitive == ti::Primitive::LINE) glDrawArrays(GL_LINES, 0, vertexcount);
		if (primitive == ti::Primitive::LINE_STRIP) glDrawArrays(GL_LINE_STRIP, 0, vertexcount);
		if (primitive == ti::Primitive::POINT) glDrawArrays(GL_POINTS, 0, vertexcount);
		engine.drawcalls++;
	}
}

void ti::System::RenderSystem::RenderSprite(int vertexcount) {
	using namespace ti::Component;

	uint32_t flags = POSITION_ATTRIB_BIT | COLOR_ATTRIB_BIT | UV0_ATTRIB_BIT;

	auto& context = registry->Store<SpriteRendererRegistry>().GetSpriteRendererContext(flags);

	context.vertexcount = vertexcount;
	SetModel(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)));

	VertexArray* vertexarray = GetVertexArray(flags);

	if (vertexarray == nullptr)
		vertexarray = GetVertexArray(flags);
	if (context.vertexbuffer == nullptr)
		context.vertexbuffer = VertexBuffer_Create();

	float* vertices = (float*)malloc(sizeof(float) * (3 + 4 + 2) * context.vertexcount);

	int i = 0;
	for (auto& entity : registry->View<Tag, Transform, SpriteRenderer>()) {
		auto& transform = registry->Get<Transform>(entity);
		auto& spriterenderer = registry->Get<SpriteRenderer>(entity);
		auto quaternion = transform.GetRotationQuat();

		if (!spriterenderer.visible) {
			context.vertexcount -= 4;
			continue;
		}

		glm::vec3 pos00 = quaternion * glm::vec3(transform.position.x - transform.scale.x, transform.position.y - transform.scale.y, transform.position.z);
		glm::vec3 pos01 = quaternion * glm::vec3(transform.position.x - transform.scale.x, transform.position.y + transform.scale.y, transform.position.z);
		glm::vec3 pos10 = quaternion * glm::vec3(transform.position.x + transform.scale.x, transform.position.y - transform.scale.y, transform.position.z);
		glm::vec3 pos11 = quaternion * glm::vec3(transform.position.x + transform.scale.x, transform.position.y + transform.scale.y, transform.position.z);

		vertices[i * vertexarray->elem_stride + vertexarray->position_offset + 0] = pos00.x;
		vertices[i * vertexarray->elem_stride + vertexarray->position_offset + 1] = pos00.y;
		vertices[i * vertexarray->elem_stride + vertexarray->position_offset + 2] = pos00.z;

		vertices[i * vertexarray->elem_stride + vertexarray->color_offset + 0] = spriterenderer.color.x;
		vertices[i * vertexarray->elem_stride + vertexarray->color_offset + 1] = spriterenderer.color.y;
		vertices[i * vertexarray->elem_stride + vertexarray->color_offset + 2] = spriterenderer.color.z;
		vertices[i * vertexarray->elem_stride + vertexarray->color_offset + 3] = spriterenderer.color.w;

		vertices[i * vertexarray->elem_stride + vertexarray->uv0_offset + 0] = 0;
		vertices[i * vertexarray->elem_stride + vertexarray->uv0_offset + 1] = 0;
		i++;

		vertices[i * vertexarray->elem_stride + vertexarray->position_offset + 0] = pos01.x;
		vertices[i * vertexarray->elem_stride + vertexarray->position_offset + 1] = pos01.y;
		vertices[i * vertexarray->elem_stride + vertexarray->position_offset + 2] = pos01.z;

		vertices[i * vertexarray->elem_stride + vertexarray->color_offset + 0] = spriterenderer.color.x;
		vertices[i * vertexarray->elem_stride + vertexarray->color_offset + 1] = spriterenderer.color.y;
		vertices[i * vertexarray->elem_stride + vertexarray->color_offset + 2] = spriterenderer.color.z;
		vertices[i * vertexarray->elem_stride + vertexarray->color_offset + 3] = spriterenderer.color.w;

		vertices[i * vertexarray->elem_stride + vertexarray->uv0_offset + 0] = 0;
		vertices[i * vertexarray->elem_stride + vertexarray->uv0_offset + 1] = 1;
		i++;

		vertices[i * vertexarray->elem_stride + vertexarray->position_offset + 0] = pos10.x;
		vertices[i * vertexarray->elem_stride + vertexarray->position_offset + 1] = pos10.y;
		vertices[i * vertexarray->elem_stride + vertexarray->position_offset + 2] = pos10.z;

		vertices[i * vertexarray->elem_stride + vertexarray->color_offset + 0] = spriterenderer.color.x;
		vertices[i * vertexarray->elem_stride + vertexarray->color_offset + 1] = spriterenderer.color.y;
		vertices[i * vertexarray->elem_stride + vertexarray->color_offset + 2] = spriterenderer.color.z;
		vertices[i * vertexarray->elem_stride + vertexarray->color_offset + 3] = spriterenderer.color.w;

		vertices[i * vertexarray->elem_stride + vertexarray->uv0_offset + 0] = 1;
		vertices[i * vertexarray->elem_stride + vertexarray->uv0_offset + 1] = 0;
		i++;

		vertices[i * vertexarray->elem_stride + vertexarray->position_offset + 0] = pos11.x;
		vertices[i * vertexarray->elem_stride + vertexarray->position_offset + 1] = pos11.y;
		vertices[i * vertexarray->elem_stride + vertexarray->position_offset + 2] = pos11.z;

		vertices[i * vertexarray->elem_stride + vertexarray->color_offset + 0] = spriterenderer.color.x;
		vertices[i * vertexarray->elem_stride + vertexarray->color_offset + 1] = spriterenderer.color.y;
		vertices[i * vertexarray->elem_stride + vertexarray->color_offset + 2] = spriterenderer.color.z;
		vertices[i * vertexarray->elem_stride + vertexarray->color_offset + 3] = spriterenderer.color.w;

		vertices[i * vertexarray->elem_stride + vertexarray->uv0_offset + 0] = 1;
		vertices[i * vertexarray->elem_stride + vertexarray->uv0_offset + 1] = 1;
		i++;
	}

	shader->SetUniformi("has_texture", 1);

	context.vertexbuffer->Allocate(sizeof(float) * (3 + 4 + 2) * context.vertexcount);
	context.vertexbuffer->AddDataDynamic(vertices, sizeof(float) * (3 + 4 + 2) * context.vertexcount);

	vertexarray->Bind();
	vertexarray->BindVertexBuffer(context.vertexbuffer, vertexarray->stride);
	vertexarray->BindIndexBuffer(TransferQuadIndices(vertexarray, context.vertexcount));

	glDrawElements(GL_TRIANGLES, context.vertexcount * 1.5, GL_UNSIGNED_INT, nullptr);
	auto& engine = registry->Store<EngineProperties>();
	engine.drawcalls++;
	engine.vertexcount += context.vertexcount;
	engine.indexcount += context.vertexcount * 1.5;

	free(vertices);
	vertices = nullptr;

	shader->SetUniformi("has_texture", 0);
}

void ti::System::RenderSystem::RenderColliders() {
	using namespace ti::Component;

	shader->SetUniformi("wireframe_mode", 1);
	shader->SetUniformVec4("wireframe_color", &glm::vec4(1, 1, 0, 1)[0]);

	static int vertexcount;
	VertexArray* vertexarray = GetVertexArray(POSITION_ATTRIB_BIT);
	static VertexBuffer* vertexbuffer;

	if (vertexbuffer == nullptr) {
		vertexbuffer = VertexBuffer_Create();
	}

	static std::vector<glm::vec3> vertices;

	for (auto& entity: registry->View<Tag, Transform, BoxCollider>()) {
		auto& transform = registry->Get<Transform>(entity);
		auto& collider = registry->Get<BoxCollider>(entity);

		if (!collider.visible) continue;

		const auto& center = collider.center + transform.position;

		vertices.reserve(12 * 2 + vertices.size());

		vertices.emplace_back(glm::vec3(center.x - collider.size.x / 2.0, center.y - collider.size.y / 2.0, center.z - collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x + collider.size.x / 2.0, center.y - collider.size.y / 2.0, center.z - collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x + collider.size.x / 2.0, center.y - collider.size.y / 2.0, center.z - collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x + collider.size.x / 2.0, center.y + collider.size.y / 2.0, center.z - collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x + collider.size.x / 2.0, center.y + collider.size.y / 2.0, center.z - collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x - collider.size.x / 2.0, center.y + collider.size.y / 2.0, center.z - collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x - collider.size.x / 2.0, center.y + collider.size.y / 2.0, center.z - collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x - collider.size.x / 2.0, center.y - collider.size.y / 2.0, center.z - collider.size.z / 2.0));

		vertices.emplace_back(glm::vec3(center.x - collider.size.x / 2.0, center.y - collider.size.y / 2.0, center.z + collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x + collider.size.x / 2.0, center.y - collider.size.y / 2.0, center.z + collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x + collider.size.x / 2.0, center.y - collider.size.y / 2.0, center.z + collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x + collider.size.x / 2.0, center.y + collider.size.y / 2.0, center.z + collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x + collider.size.x / 2.0, center.y + collider.size.y / 2.0, center.z + collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x - collider.size.x / 2.0, center.y + collider.size.y / 2.0, center.z + collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x - collider.size.x / 2.0, center.y + collider.size.y / 2.0, center.z + collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x - collider.size.x / 2.0, center.y - collider.size.y / 2.0, center.z + collider.size.z / 2.0));

		vertices.emplace_back(glm::vec3(center.x + collider.size.x / 2.0, center.y + collider.size.y / 2.0, center.z - collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x + collider.size.x / 2.0, center.y + collider.size.y / 2.0, center.z + collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x - collider.size.x / 2.0, center.y + collider.size.y / 2.0, center.z - collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x - collider.size.x / 2.0, center.y + collider.size.y / 2.0, center.z + collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x + collider.size.x / 2.0, center.y - collider.size.y / 2.0, center.z - collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x + collider.size.x / 2.0, center.y - collider.size.y / 2.0, center.z + collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x - collider.size.x / 2.0, center.y - collider.size.y / 2.0, center.z - collider.size.z / 2.0));
		vertices.emplace_back(glm::vec3(center.x - collider.size.x / 2.0, center.y - collider.size.y / 2.0, center.z + collider.size.z / 2.0));
	}

	vertexbuffer->Allocate(sizeof(glm::vec3)*vertices.size());
	vertexbuffer->AddDataDynamic(vertices.data(), sizeof(glm::vec3)*vertices.size());

	vertexarray->Bind();
	vertexarray->BindVertexBuffer(vertexbuffer, vertexarray->stride);

	glDrawArrays(GL_LINES, 0, vertices.size());

	vertices.clear();

	shader->SetUniformi("wireframe_mode", 0);
}

void ti::System::RenderSystem::Render(WindowContext& window) {
	auto& events = registry->Store<ti::Events>();
	auto& engine = registry->Store<ti::EngineProperties>();

	if (engine.debug_mode)
		window.framebuffer->Bind();

	if (uniformbuffer == nullptr) {
		uniformbuffer = UniformBuffer_Create();
		uniformbuffer->Allocate(sizeof(glm::mat4) * 4);
		uniformbuffer->BindRange(0, sizeof(glm::mat4) * 4);
	}

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, window.width, window.height);
	glClearColor(0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	using namespace ti::Component;

	if (window.type == EditorWindow) {	// Editor Camera
		auto& editorcamera = window.camera;
		if (editorcamera == 0) {
			
		}

		auto& camera = registry->Get<Camera>(editorcamera);
		auto& transform = registry->Get<Transform>(editorcamera);

		SetViewPosition(transform.position);
		SetView(camera.view);
		SetProjection(camera.projection);
	}

	SetShader(registry->Store<ShaderRegistry>().GetShader(0, registry));

	int point_light = 0;
	int dir_light = 0;

	for (auto& entity : registry->View<Tag, Transform, Light>()) {
		auto& transform = registry->Get<Transform>(entity);
		auto& light = registry->Get<Light>(entity);

		if (!light.active) continue;

		if (light.mode == Directional) {
			auto direction = glm::normalize(transform.GetRotationQuat() * glm::vec3(0, -1.0, 0));

			shader->SetUniformVec3("dir_light" + std::to_string(dir_light) + ".color", &light.color[0]);
			shader->SetUniformVec3("dir_light" + std::to_string(dir_light) + ".direction", &direction[0]);

			dir_light++;
		}
		if (light.mode == Point) {
			shader->SetUniformVec3("point_light" + std::to_string(point_light) + ".color", &light.color[0]);
			shader->SetUniformVec3("point_light" + std::to_string(point_light) + ".position", &transform.position[0]);

			point_light++;
		}
	}

	for (auto& entity : registry->View<Tag, Transform, MeshFilter, MeshRenderer>()) {
		auto meshfilter = registry->Get<MeshFilter>(entity);
		if (!registry->Store<MeshRegistry>().Contains(meshfilter.mesh)) continue;

		auto& mesh = registry->Store<MeshRegistry>().GetMesh(meshfilter.mesh);
		auto& meshrenderer = registry->Get<MeshRenderer>(entity);
		auto& transform = registry->Get<Transform>(entity);

		if (!meshrenderer.visible) continue;

		if (!registry->Store<MaterialRegistry>().Contains(meshrenderer.material)) continue;

		SetModel(transform.GetModel());

		TransferMesh(mesh, meshrenderer);
		Render(TRIANGLE, meshrenderer.material, meshrenderer.vertexarray, mesh.vertexcount, mesh.vertexbuffer, mesh.indexcount, mesh.indexbuffer);
	}

	for (auto& entity : registry->View<Tag, Transform, Mesh, MeshRenderer>()) {
		auto& mesh = registry->Get<Mesh>(entity);
		auto& meshrenderer = registry->Get<MeshRenderer>(entity);
		auto& transform = registry->Get<Transform>(entity);

		if (!meshrenderer.visible) continue;

		if (!registry->Store<MaterialRegistry>().Contains(meshrenderer.material)) continue;

		SetModel(transform.GetModel());

		TransferMesh(mesh, meshrenderer);
		Render(TRIANGLE, meshrenderer.material, meshrenderer.vertexarray, mesh.vertexcount, mesh.vertexbuffer, mesh.indexcount, mesh.indexbuffer);
	}

	RenderSprite(registry->View<Tag, Transform, SpriteRenderer>().size() * 4);
	RenderColliders();

	if (engine.debug_mode)
		window.framebuffer->UnBind();
}
