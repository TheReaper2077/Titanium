#pragma once

#include "Components/Components.h"
#include <OpenGL.h>
#include <regex>
#include "ECS.h"
#include <string>
#include <fstream>

#define SHADER_LIGHTING_BIT 0x01
#define SHADER_TEXTURE_BIT 0x02
#define SHADER_COLOR_BIT 0x04

static std::string read_file(const std::string &filename) {
	std::string line, text;
	std::fstream file(filename);

	while (std::getline(file, line)) text += (line + '\n');

	file.close();

	return text;
}

namespace ti {
	struct ShaderRegistry {
		std::unordered_map<std::string, Shader*> registry;

		Shader* GetShader(uint32_t flags, ti::ECS::Registry* registry);

		void AddShader(Shader* shader);

		Shader* GetShader(std::string name);

		Shader* RegisterShader(uint32_t shader_flags, int point_light, int dir_light);

		std::string SetUniforms(int point_light, int dir_light);

		std::string SetFunctions(int point_light, int dir_light);

		std::string SetLogic(int point_light, int dir_light);
	};
}
