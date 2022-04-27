#pragma once

#include "System.h"

namespace ti {
	namespace System {
		class RenderSystem: System {
			UniformBuffer* uniformbuffer;

			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 projection;

			void Init() override {
				uniformbuffer = UniformBuffer_Create();
				uniformbuffer->Allocate(sizeof(glm::mat4) * 3);
				uniformbuffer->BindRange(0, sizeof(glm::mat4) * 3);
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

			void Update(double dt) override {
				using namespace ti::Component;

				for (auto& entity: registry->View<Properties, Transform, Camera>()) {
					auto& camera = registry->Get<Camera>(entity);

					SetView(camera.view);
				}

				for (auto& entity: registry->View<Properties, Transform, MeshRenderer>()) {
					
				}
			}
		};
	}
}