#pragma once

#include "System.h"
#include <unordered_map>
#include <iostream>
#include "../MaterialRegistry.h"

namespace ti {
	namespace System {
		class RenderSystem: public System {
			UniformBuffer* uniformbuffer;

			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 projection;
			glm::vec3 view_position;

			std::unordered_map<std::size_t, VertexArray*> vertexarray_registry;
			std::unordered_map<std::string, Shader*> shader_registry;

			Shader* shader;

			std::vector<uint32_t> indices;
			uint32_t indexcount;
			std::vector<Vertex> vertices;
			uint32_t vertexcount;
		
		public:
			RenderSystem() {}

			void Init() override {
				uniformbuffer = UniformBuffer_Create();
				uniformbuffer->Allocate(sizeof(glm::mat4) * 4);
				uniformbuffer->BindRange(0, sizeof(glm::mat4) * 4);

				auto* shader = Shader_Create("material", "D:\\C++\\2.5D Engine\\src\\Shaders\\default.vs", "D:\\C++\\2.5D Engine\\src\\Shaders\\color.fs");
				// shader->BindUniformBlock("ProjectionMatrix", 0);
				RegisterShader(shader);

				RegisterVertexArray<Vertex>(
					{
						{ position, 0, 3, GL_FLOAT },
						{ uv0, 1, 3, GL_FLOAT },
						{ normal, 2, 3, GL_FLOAT },
					}
				);
			}

			template <typename T>
			void RegisterVertexArray(std::vector<VertexArrayAttribDescriptor> vertexarraydesc) {
				assert(vertexarray_registry.find(typeid(T).hash_code()) == vertexarray_registry.end());

				vertexarray_registry[typeid(T).hash_code()] = VertexArray_Create(vertexarraydesc);
			}

			void RegisterShader(Shader* shader) {
				assert(shader_registry.find(shader->name) == shader_registry.end());

				shader_registry[shader->name] = shader;
			}

			void SetShader(Shader* shader) {
				this->shader = shader;
			}

			void SetShader(std::string name) {
				SetShader(GetShader(name));
			}

			Shader* GetShader(std::string name) {
				return shader_registry[name];
			}

			template <typename T>
			void SetMeshVertexArray(ti::Component::MeshFilter& mesh, std::vector<T> vertices) {
				if (mesh.vertexarray != nullptr) return;
				
				assert(vertexarray_registry.find(typeid(T).hash_code()) != vertexarray_registry.end());

				mesh.vertexarray = vertexarray_registry[typeid(T).hash_code()];

				if (mesh.vertexbuffer == nullptr)
					mesh.vertexbuffer = VertexBuffer_Create();
				if (mesh.indexbuffer == nullptr)
					mesh.indexbuffer = IndexBuffer_Create(mesh.vertexarray);
			}

			void SetMaterial(std::string name) {
				SetMaterial(registry->Store<MaterialRegistry>().GetMaterial(name));
			}

			void SetMaterial(ti::Component::Material& material) {
				// SetShader("material");

				shader->SetUniformVec3("material.ambient", &material.ambient[0]);
				shader->SetUniformVec3("material.diffuse", &material.diffuse[0]);
				shader->SetUniformVec3("material.specular", &material.specular[0]);
				shader->SetUniformf("material.shininess", material.shininess);

				if (material.ambient_map != nullptr) {
					glActiveTexture(GL_TEXTURE0);
					material.ambient_map->BindUnit(0);
					shader->SetUniformi("material.ambient_map", 0);
				}

				if (material.diffuse_map != nullptr) {
					glActiveTexture(GL_TEXTURE1);
					material.diffuse_map->BindUnit(1);
					shader->SetUniformi("material.diffuse_map", 1);
				}

				if (material.specular_map != nullptr) {
					glActiveTexture(GL_TEXTURE2);
					material.specular_map->BindUnit(2);
					shader->SetUniformi("material.specular_map", 2);
				}
			}

			template <typename T>
			void TransferVertices(const std::vector<T>& vertices, VertexBuffer* vertexbuffer) {
				if (vertices.size())
					vertexbuffer->AddDataStatic((void*)vertices.data(), vertices.size()*sizeof(T));
			}

			void TransferIndices(std::vector<uint32_t>& indices, IndexBuffer* indexbuffer) {
				if (indices.size())
					indexbuffer->AddData(indices.data(), indices.size()*sizeof(uint32_t));
			}

			void TransferMesh(ti::Component::MeshFilter& mesh) {
				TransferVertices(mesh.vertices, mesh.vertexbuffer);
				TransferIndices(mesh.indices, mesh.indexbuffer);

				mesh.vertexcount = mesh.vertices.size();
				mesh.indexcount = mesh.indices.size();
				mesh.changed = false;
			}

			void SetModel(const glm::mat4& model) {
				if (this->model == model) return;
				this->model = model;
				uniformbuffer->AddDataDynamic(&this->model[0][0], sizeof(glm::mat4), sizeof(glm::mat4) * 0);
			}

			void SetView(const glm::mat4& view) {
				if (this->view == view) return;
				this->view = view;
				uniformbuffer->AddDataDynamic(&this->view[0][0], sizeof(glm::mat4), sizeof(glm::mat4) * 1);
			}

			void SetProjection(const glm::mat4& projection) {
				if (this->projection == projection) return;
				this->projection = projection;
				uniformbuffer->AddDataDynamic(&this->projection[0][0], sizeof(glm::mat4), sizeof(glm::mat4) * 2);
			}

			void SetViewPosition(const glm::vec3& view_position) {
				if (this->view_position == view_position) return;
				this->view_position = view_position;
				uniformbuffer->AddDataDynamic(&this->view_position[0], sizeof(glm::vec3), sizeof(glm::mat4) * 3);
			}

			void RenderMesh(ti::Component::MeshFilter& mesh) {
				auto& engine = registry->Store<EngineProperties>();

				if (mesh.vertexarray == nullptr)
					SetMeshVertexArray(mesh, mesh.vertices);

				SetMaterial(mesh.material);

				if (mesh.changed) {
					TransferMesh(mesh);
				}
				
				mesh.vertexarray->Bind();
				mesh.vertexarray->BindVertexBuffer(mesh.vertexbuffer, mesh.vertexarray->stride);
				engine.vertexcount += mesh.vertexcount;

				if (mesh.indexcount) {
					mesh.vertexarray->BindIndexBuffer(mesh.indexbuffer);
					engine.indexcount += mesh.indexcount;
					
					if (mesh.primitive == ti::Primitive::TRIANGLE) glDrawElements(GL_TRIANGLES, mesh.indexcount, GL_UNSIGNED_INT, nullptr);
					if (mesh.primitive == ti::Primitive::TRIANGLE_FAN) glDrawElements(GL_TRIANGLE_FAN, mesh.indexcount, GL_UNSIGNED_INT, nullptr);
					if (mesh.primitive == ti::Primitive::TRIANGLE_STRIP) glDrawElements(GL_TRIANGLE_STRIP, mesh.indexcount, GL_UNSIGNED_INT, nullptr);
					if (mesh.primitive == ti::Primitive::LINE) glDrawElements(GL_LINES, mesh.indexcount, GL_UNSIGNED_INT, nullptr);
					if (mesh.primitive == ti::Primitive::LINE_STRIP) glDrawElements(GL_LINE_STRIP, mesh.indexcount, GL_UNSIGNED_INT, nullptr);
					if (mesh.primitive == ti::Primitive::POINT) glDrawElements(GL_POINTS, mesh.indexcount, GL_UNSIGNED_INT, nullptr);

					engine.drawcalls++;
				} else {
					if (mesh.primitive == ti::Primitive::TRIANGLE) glDrawArrays(GL_TRIANGLES, 0, mesh.vertexcount);
					if (mesh.primitive == ti::Primitive::TRIANGLE_FAN) glDrawArrays(GL_TRIANGLE_FAN, 0, mesh.vertexcount);
					if (mesh.primitive == ti::Primitive::TRIANGLE_STRIP) glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh.vertexcount);
					if (mesh.primitive == ti::Primitive::LINE) glDrawArrays(GL_LINES, 0, mesh.vertexcount);
					if (mesh.primitive == ti::Primitive::LINE_STRIP) glDrawArrays(GL_LINE_STRIP, 0, mesh.vertexcount);
					if (mesh.primitive == ti::Primitive::POINT) glDrawArrays(GL_POINTS, 0, mesh.vertexcount);
					engine.drawcalls++;
				}
			}

			void Update(double dt) override {
				using namespace ti::Component;

				for (auto& entity: registry->View<Tag, Transform, Camera>()) {
					auto& camera = registry->Get<Camera>(entity);
					auto& transform = registry->Get<Transform>(entity);

					if (!camera.enable) continue;

					SetViewPosition(transform.position);
					SetView(camera.view);
					SetProjection(camera.projection);
				}

				for (auto& entity: registry->View<Tag, Transform, MeshFilter>()) {
					auto& mesh = registry->Get<MeshFilter>(entity);
					auto& transform = registry->Get<Transform>(entity);

					SetShader("material");
					SetModel(transform.GetModel());
					RenderMesh(mesh);
				}

				for (auto& entity: registry->View<Tag, Transform, Model>()) {
					auto& model = registry->Get<Model>(entity);
					auto& transform = registry->Get<Transform>(entity);

					SetShader("material");
					
					shader->SetUniformVec3("camera_pos", &view_position[0]);
					shader->SetUniformVec3("light.position", &glm::vec3(1.2f, 1000.0f, 2.0f)[0]);
					shader->SetUniformVec3("light.direction", &glm::vec3(-0.2f, -1.0f, -0.3f)[0]);
					shader->SetUniformVec3("light.ambient", &glm::vec3(0.2f, 0.2f, 0.2f)[0]);
					shader->SetUniformVec3("light.diffuse", &glm::vec3(0.5f, 0.5f, 0.5f)[0]);
					shader->SetUniformVec3("light.specular", &glm::vec3(1.0f, 1.0f, 1.0f)[0]);
					shader->SetUniformi("light.constant", 1.0f);
					shader->SetUniformi("light.linear", 0.09f);
					shader->SetUniformi("light.quadratic", 0.032f);

					SetModel(transform.GetModel());

					for (auto& mesh: model.meshes) {
						RenderMesh(mesh);
					}
				}

				// shader->UnBind();

				// for (auto& entity: registry->View<Properties, Transform, Sprite>()) {
				// 	auto& sprite = registry->Get<Sprite>(entity);
				// 	auto& transform = registry->Get<Transform>(entity);

				// 	SetModel(transform.GetModel());
				// 	SetShader("sprite");
				// }

				// for (auto& entity: registry->View<Properties, Transform, Texture>()) {
				// 	auto& sprite = registry->Get<Sprite>(entity);
				// 	auto& transform = registry->Get<Transform>(entity);

				// 	SetModel(transform.GetModel());
				// 	SetShader("texture");
				// }
			}
		};
	}
}