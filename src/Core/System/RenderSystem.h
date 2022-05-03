#pragma once

#include "../ShaderRegistry.h"
#include "System.h"
#include <unordered_map>
#include <iostream>
#include "../Functions.h"
#include "../MaterialRegistry.h"
#include "../MeshRegistry.h"

namespace ti {
	namespace System {
		class RenderSystem: public System {
			UniformBuffer* uniformbuffer;

			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 projection;
			glm::vec3 view_position;

			std::unordered_map<uint32_t, VertexArray*> vertexarray_registry;

			Shader* shader;
		
		public:
			RenderSystem() {}

			void Init(ti::ECS::Registry *registry) override {
				this->registry = registry;
				
				uniformbuffer = UniformBuffer_Create();
				uniformbuffer->Allocate(sizeof(glm::mat4) * 4);
				uniformbuffer->BindRange(0, sizeof(glm::mat4) * 4);
			}

			VertexArray* GetVertexArray(uint32_t flags) {
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

			void SetShader(Shader* shader) {
				this->shader = shader;
			}

			void SetMaterial(std::string name) {
				SetMaterial(registry->Store<MaterialRegistry>().GetMaterial(name));
			}

			void SetMaterial(ti::Component::Material& material) {
				shader->SetUniformVec3("material.ambient", &material.ambient[0]);
				shader->SetUniformVec3("material.diffuse", &material.diffuse[0]);
				shader->SetUniformVec3("material.specular", &material.specular[0]);
				shader->SetUniformf("material.shininess", material.shininess);

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

				if (!mesh.changed && mesh.indices.size() == mesh.indexcount && mesh.positions.size() == mesh.vertexcount && meshrenderer.flags == flags) return;
				mesh.changed = true;
				meshrenderer.flags = flags;

				mesh.indexcount = mesh.indices.size();
				mesh.vertexcount = mesh.positions.size();

				if (meshrenderer.vertexarray == nullptr)
					meshrenderer.vertexarray = GetVertexArray(meshrenderer.flags);
				
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

				mesh.vertexbuffer->Allocate(mesh.vertexcount*meshrenderer.vertexarray->stride);
				mesh.vertexbuffer->AddDataDynamic((void*)data, mesh.vertexcount*meshrenderer.vertexarray->stride);
				// mesh.vertexbuffer->AddDataStatic((void*)data, mesh.vertexcount*meshrenderer.vertexarray->stride);
				mesh.indexbuffer->AddData(mesh.indices.data(), sizeof(uint32_t)*mesh.indexcount);

				mesh.changed = false;
				free(data);
				data = nullptr;
			}

			void Render(Primitive primitive, std::string material, VertexArray* vertexarray, int vertexcount, VertexBuffer* vertexbuffer, int indexcount = 0, IndexBuffer* indexbuffer = nullptr) {
				auto& engine = registry->Store<EngineProperties>();
				SetMaterial(material);
				shader->Bind();
				
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

			void SetRenderPass(bool debug) {
				registry->Store<EngineProperties>().debug_render = debug;
			}

			void Update(double dt) override {
				using namespace ti::Component;

				if (registry->Store<EngineProperties>().debug_render) {	// Editor Camera
					auto& editorcamera = registry->Store<Functions>().editor_camera;
					if (editorcamera == 0) {
						editorcamera = registry->Create();
						registry->Add<Transform>(editorcamera);
						registry->Add<Camera>(editorcamera, PERSPECTIVE, true, Editor);
					}

					auto& camera = registry->Get<Camera>(editorcamera);
					auto& transform = registry->Get<Transform>(editorcamera);

					SetViewPosition(transform.position);
					SetView(camera.view);
					SetProjection(camera.projection);
				} else {
					for (auto& entity: registry->View<Tag, Transform, Camera>()) {
						auto& camera = registry->Get<Camera>(entity);
						auto& transform = registry->Get<Transform>(entity);
						
						if (!camera.enable) continue;

						SetViewPosition(transform.position);
						SetView(camera.view);
						SetProjection(camera.projection);
					}
				}

				SetShader(registry->Store<ShaderRegistry>().GetShader(registry));

				int point_light = 0;
				int dir_light = 0;
				int flash_light = 0;
				int spot_light = 0;
				int area_light = 0;

				for (auto& entity: registry->View<Tag, Transform, Light>()) {
					auto& transform = registry->Get<Transform>(entity);
					auto& light = registry->Get<Light>(entity);

					if (!light.active) continue;

					if (light.mode == Directional) {
						auto direction = glm::normalize(transform.GetRotationQuat() * glm::vec3(0, -1.0, 0));
						
						shader->SetUniformVec3("directional" + std::to_string(dir_light) + ".direction", &direction[0]);
						shader->SetUniformVec3("directional" + std::to_string(dir_light) + ".ambient", &light.ambient[0]);
						shader->SetUniformVec3("directional" + std::to_string(dir_light) + ".diffuse", &light.diffuse[0]);
						shader->SetUniformVec3("directional" + std::to_string(dir_light) + ".specular", &light.specular[0]);

						dir_light++;
					}
					if (light.mode == Point) {
						shader->SetUniformVec3("point" + std::to_string(point_light) + ".position", &transform.position[0]);
						shader->SetUniformVec3("point" + std::to_string(point_light) + ".ambient", &light.ambient[0]);
						shader->SetUniformVec3("point" + std::to_string(point_light) + ".diffuse", &light.diffuse[0]);
						shader->SetUniformVec3("point" + std::to_string(point_light) + ".specular", &light.specular[0]);

						shader->SetUniformVec3("point" + std::to_string(point_light) + ".constant", &light.constant);
						shader->SetUniformVec3("point" + std::to_string(point_light) + ".linear", &light.linear);
						shader->SetUniformVec3("point" + std::to_string(point_light) + ".quadratic", &light.quadratic);

						point_light++;
					}
				}

				for (auto& entity: registry->View<Tag, Transform, MeshFilter, MeshRenderer>()) {
					auto meshfilter = registry->Get<MeshFilter>(entity);
					if (!registry->Store<MeshRegistry>().Contains(meshfilter.mesh)) continue;
					
					auto& mesh = registry->Store<MeshRegistry>().GetMesh(meshfilter.mesh);
					auto& meshrenderer = registry->Get<MeshRenderer>(entity);
					auto& transform = registry->Get<Transform>(entity);

					if (!registry->Store<MaterialRegistry>().Contains(meshrenderer.material)) continue;

					SetModel(transform.GetModel());
					
					TransferMesh(mesh, meshrenderer);
					Render(TRIANGLE, meshrenderer.material, meshrenderer.vertexarray, mesh.vertexcount, mesh.vertexbuffer, mesh.indexcount, mesh.indexbuffer);
				}

				for (auto& entity: registry->View<Tag, Transform, Mesh, MeshRenderer>()) {
					auto& mesh = registry->Get<Mesh>(entity);
					auto& meshrenderer = registry->Get<MeshRenderer>(entity);
					auto& transform = registry->Get<Transform>(entity);

					if (!registry->Store<MaterialRegistry>().Contains(meshrenderer.material)) continue;

					SetModel(transform.GetModel());
					
					TransferMesh(mesh, meshrenderer);
					Render(TRIANGLE, meshrenderer.material, meshrenderer.vertexarray, mesh.vertexcount, mesh.vertexbuffer, mesh.indexcount, mesh.indexbuffer);
				}

				for (auto& entity: registry->View<Tag, Transform, SpriteRenderer>()) {
					auto& transform = registry->Get<Transform>(entity);
					auto& spriterenderer = registry->Get<SpriteRenderer>(entity);

					
				}
			}
		};
	}
}