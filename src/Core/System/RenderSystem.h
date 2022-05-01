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

			std::unordered_map<uint32_t, VertexArray*> vertexarray_registry;
			std::unordered_map<std::string, Shader*> shader_registry;

			Shader* shader;
		
		public:
			RenderSystem() {}

			void Init() override {
				uniformbuffer = UniformBuffer_Create();
				uniformbuffer->Allocate(sizeof(glm::mat4) * 4);
				uniformbuffer->BindRange(0, sizeof(glm::mat4) * 4);

				auto* shader = Shader_Create("material", "D:\\C++\\2.5D Engine\\src\\Shaders\\default.vs", "D:\\C++\\2.5D Engine\\src\\Shaders\\color.fs");
				// shader->BindUniformBlock("ProjectionMatrix", 0);
				RegisterShader(shader);
			}

			VertexArray* GetVertexArray(uint32_t flags) {
				if (vertexarray_registry.find(flags) == vertexarray_registry.end()) {
					std::vector<VertexArrayAttribDescriptor> descriptor;

					std::cout << std::bitset<16>(NORMAL_ATTRIB_BIT) << ' ' << std::bitset<16>(NORMAL_ATTRIB_BIT & flags) << '\n';

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

			void TransferMesh(ti::Component::Mesh& mesh, ti::Component::MeshRenderer& meshrenderer) {
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

				if (!mesh.changed && mesh.indices.size() == meshrenderer.indexcount && mesh.positions.size() == meshrenderer.vertexcount && meshrenderer.flags == flags) return;
				mesh.changed = true;
				meshrenderer.flags = flags;

				meshrenderer.indexcount = mesh.indices.size();
				meshrenderer.vertexcount = mesh.positions.size();

				if (meshrenderer.vertexarray == nullptr)
					meshrenderer.vertexarray = GetVertexArray(meshrenderer.flags);
				
				if (meshrenderer.vertexbuffer == nullptr && meshrenderer.vertexcount)
					meshrenderer.vertexbuffer = VertexBuffer_Create();

				if (meshrenderer.indexbuffer == nullptr && meshrenderer.indexcount)
					meshrenderer.indexbuffer = IndexBuffer_Create(meshrenderer.vertexarray);

				meshrenderer.data = (float*)malloc(meshrenderer.vertexcount * meshrenderer.vertexarray->stride);

				for (int i = 0; i < meshrenderer.vertexcount; i++) {
					if (meshrenderer.vertexarray->has_position) {
						if (i < mesh.positions.size()) {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->position_offset + 0] = mesh.positions[i].x;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->position_offset + 1] = mesh.positions[i].y;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->position_offset + 2] = mesh.positions[i].z;
						} else {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->position_offset + 0] = 0;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->position_offset + 1] = 0;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->position_offset + 2] = 0;
						}
					}
					if (meshrenderer.vertexarray->has_normal) {
						if (i < mesh.normals.size()) {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->normal_offset + 0] = mesh.normals[i].x;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->normal_offset + 1] = mesh.normals[i].y;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->normal_offset + 2] = mesh.normals[i].z;
						} else {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->normal_offset + 0] = 0;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->normal_offset + 1] = 0;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->normal_offset + 2] = 0;
						}
					}
					if (meshrenderer.vertexarray->has_color) {
						if (i < mesh.color.size()) {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->color_offset + 0] = mesh.color[i].x;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->color_offset + 1] = mesh.color[i].y;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->color_offset + 2] = mesh.color[i].z;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->color_offset + 3] = mesh.color[i].w;
						} else {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->color_offset + 0] = 0;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->color_offset + 1] = 0;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->color_offset + 2] = 0;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->color_offset + 3] = 0;
						}
					}
					if (meshrenderer.vertexarray->has_uv0) {
						if (i < mesh.uv0.size()) {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv0_offset + 0] = mesh.uv0[i].x;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv0_offset + 1] = mesh.uv0[i].y;
						} else {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv0_offset + 0] = 0;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv0_offset + 1] = 0;
						}
					}
					if (meshrenderer.vertexarray->has_uv1) {
						if (i < mesh.uv1.size()) {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv1_offset + 0] = mesh.uv1[i].x;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv1_offset + 1] = mesh.uv1[i].y;
						} else {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv1_offset + 0] = 0;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv1_offset + 1] = 0;
						}
					}
					if (meshrenderer.vertexarray->has_uv2) {
						if (i < mesh.uv2.size()) {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv2_offset + 0] = mesh.uv2[i].x;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv2_offset + 1] = mesh.uv2[i].y;
						} else {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv2_offset + 0] = 0;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv2_offset + 1] = 0;
						}
					}
					if (meshrenderer.vertexarray->has_uv3) {
						if (i < mesh.uv3.size()) {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv3_offset + 0] = mesh.uv3[i].x;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv3_offset + 1] = mesh.uv3[i].y;
						} else {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv3_offset + 0] = 0;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv3_offset + 1] = 0;
						}
					}
					if (meshrenderer.vertexarray->has_uv4) {
						if (i < mesh.uv4.size()) {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv4_offset + 0] = mesh.uv4[i].x;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv4_offset + 1] = mesh.uv4[i].y;
						} else {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv4_offset + 0] = 0;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv4_offset + 1] = 0;
						}
					}
					if (meshrenderer.vertexarray->has_uv5) {
						if (i < mesh.uv5.size()) {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv5_offset + 0] = mesh.uv5[i].x;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv5_offset + 1] = mesh.uv5[i].y;
						} else {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv5_offset + 0] = 0;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv5_offset + 1] = 0;
						}
					}
					if (meshrenderer.vertexarray->has_uv6) {
						if (i < mesh.uv6.size()) {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv6_offset + 0] = mesh.uv6[i].x;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv6_offset + 1] = mesh.uv6[i].y;
						} else {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv6_offset + 0] = 0;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv6_offset + 1] = 0;
						}
					}
					if (meshrenderer.vertexarray->has_uv7) {
						if (i < mesh.uv7.size()) {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv7_offset + 0] = mesh.uv7[i].x;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv7_offset + 1] = mesh.uv7[i].y;
						} else {
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv7_offset + 0] = 0;
							meshrenderer.data[i * meshrenderer.vertexarray->elem_stride + meshrenderer.vertexarray->uv7_offset + 1] = 0;
						}
					}
				}

				// meshrenderer.vertexbuffer->Allocate(meshrenderer.vertexcount*meshrenderer.vertexarray->stride);
				// meshrenderer.vertexbuffer->AddDataDynamic((void*)meshrenderer.data, meshrenderer.vertexcount*meshrenderer.vertexarray->stride);
				meshrenderer.vertexbuffer->AddDataStatic((void*)meshrenderer.data, meshrenderer.vertexcount*meshrenderer.vertexarray->stride);
				meshrenderer.indexbuffer->AddData(mesh.indices.data(), sizeof(uint32_t)*meshrenderer.indexcount);

				mesh.changed = false;
				free(meshrenderer.data);
				meshrenderer.data = nullptr;
			}

			void RenderMesh(ti::Component::MeshRenderer& meshrenderer) {
				auto& engine = registry->Store<EngineProperties>();
				SetMaterial(meshrenderer.material);
				shader->Bind();
				
				meshrenderer.vertexarray->Bind();
				meshrenderer.vertexarray->BindVertexBuffer(meshrenderer.vertexbuffer, meshrenderer.vertexarray->stride);
				engine.vertexcount += meshrenderer.vertexcount;

				if (meshrenderer.indexcount) {
					meshrenderer.vertexarray->BindIndexBuffer(meshrenderer.indexbuffer);
					engine.indexcount += meshrenderer.indexcount;
					
					if (meshrenderer.primitive == ti::Primitive::TRIANGLE) glDrawElements(GL_TRIANGLES, meshrenderer.indexcount, GL_UNSIGNED_INT, nullptr);
					if (meshrenderer.primitive == ti::Primitive::TRIANGLE_FAN) glDrawElements(GL_TRIANGLE_FAN, meshrenderer.indexcount, GL_UNSIGNED_INT, nullptr);
					if (meshrenderer.primitive == ti::Primitive::TRIANGLE_STRIP) glDrawElements(GL_TRIANGLE_STRIP, meshrenderer.indexcount, GL_UNSIGNED_INT, nullptr);
					if (meshrenderer.primitive == ti::Primitive::LINE) glDrawElements(GL_LINES, meshrenderer.indexcount, GL_UNSIGNED_INT, nullptr);
					if (meshrenderer.primitive == ti::Primitive::LINE_STRIP) glDrawElements(GL_LINE_STRIP, meshrenderer.indexcount, GL_UNSIGNED_INT, nullptr);
					if (meshrenderer.primitive == ti::Primitive::POINT) glDrawElements(GL_POINTS, meshrenderer.indexcount, GL_UNSIGNED_INT, nullptr);
					engine.drawcalls++;
				} else {
					if (meshrenderer.primitive == ti::Primitive::TRIANGLE) glDrawArrays(GL_TRIANGLES, 0, meshrenderer.vertexcount);
					if (meshrenderer.primitive == ti::Primitive::TRIANGLE_FAN) glDrawArrays(GL_TRIANGLE_FAN, 0, meshrenderer.vertexcount);
					if (meshrenderer.primitive == ti::Primitive::TRIANGLE_STRIP) glDrawArrays(GL_TRIANGLE_STRIP, 0, meshrenderer.vertexcount);
					if (meshrenderer.primitive == ti::Primitive::LINE) glDrawArrays(GL_LINES, 0, meshrenderer.vertexcount);
					if (meshrenderer.primitive == ti::Primitive::LINE_STRIP) glDrawArrays(GL_LINE_STRIP, 0, meshrenderer.vertexcount);
					if (meshrenderer.primitive == ti::Primitive::POINT) glDrawArrays(GL_POINTS, 0, meshrenderer.vertexcount);
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

				for (auto& entity: registry->View<Tag, Transform, Mesh, MeshRenderer>()) {
					auto& mesh = registry->Get<Mesh>(entity);
					auto& meshrenderer = registry->Get<MeshRenderer>(entity);
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
					
					TransferMesh(mesh, meshrenderer);
					RenderMesh(meshrenderer);
				}
			}
		};
	}
}