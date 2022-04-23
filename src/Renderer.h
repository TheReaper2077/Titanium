#pragma once

#include <glm/mat4x4.hpp>
#include <unordered_map>
#include <string>

#include "Mesh.h"
#include "OpenGL/OpenGL.h"

namespace ti {
	class Renderer {
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 model;

		std::unordered_map<std::string, Shader*> shader_map;

		UniformBuffer *uniformbuffer;

	public:
		void Init();
		Shader* AddShader(std::string name, Shader* shader);
		Shader* GetShader(std::string name);
		void SetModel(const glm::mat4& model);
		void SetView(const glm::mat4& view);
		void SetProjection(const glm::mat4& projection);
		void RenderMesh(Mesh* mesh, Shader* shader);
	};
}