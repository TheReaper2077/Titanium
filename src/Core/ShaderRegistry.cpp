#include "ShaderRegistry.h"

Shader* ti::ShaderRegistry::GetShader(uint32_t flags, ti::ECS::Registry* registry) {
	using namespace ti::Component;

	int point_light = 0;
	int dir_light = 0;

	for (auto& entity : registry->View<Tag, Transform, Light>()) {
		auto& light = registry->Get<Light>(entity);

		if (!light.active) continue;

		if (light.active) {
			if (light.mode == Point) point_light++;
			if (light.mode == Directional) dir_light++;
		}
	}

	return RegisterShader(flags, point_light, dir_light);
}

void ti::ShaderRegistry::AddShader(Shader* shader) {
	registry[shader->name] = shader;
}

Shader* ti::ShaderRegistry::GetShader(std::string name) {
	return registry[name];
}

void replace_first(std::string& s, std::string const& toReplace, std::string const& replaceWith) {
    std::size_t pos = s.find(toReplace);
    if (pos == std::string::npos) return;

    s.replace(pos, toReplace.length(), replaceWith);
}

Shader* ti::ShaderRegistry::RegisterShader(uint32_t shader_flags, int point_light, int dir_light) {
	std::string id = std::to_string(shader_flags) + '_' + std::to_string(dir_light) + '_' + std::to_string(point_light);

	if (registry.find(id) == registry.end()) {
		std::string vertexshader = read_file("D:\\C++\\2.5D Engine\\src\\Shaders\\default.vs");
		std::string fragmentshader = read_file("D:\\C++\\2.5D Engine\\src\\Shaders\\pbr.fs");

		replace_first(fragmentshader, "#define SET_LOGIC", SetLogic(point_light, dir_light));
		replace_first(fragmentshader, "#define SET_UNIFORMS", SetUniforms(point_light, dir_light));
		replace_first(fragmentshader, "#define SET_FUNCTIONS", SetFunctions(point_light, dir_light));

		std::cout << fragmentshader << '\n';

		registry[id] = Shader_Create(id, vertexshader, fragmentshader, false);
	}

	return registry[id];
}

std::string ti::ShaderRegistry::SetUniforms(int point_light, int dir_light) {
	std::string uniforms;

	for (int i = 0; i < point_light; i++)
		uniforms += "uniform Light point_light" + std::to_string(i) + ";\n";
	for (int i = 0; i < dir_light; i++)
		uniforms += "uniform Light dir_light" + std::to_string(i) + ";\n";

	return uniforms;
}

std::string ti::ShaderRegistry::SetFunctions(int point_light, int dir_light) {
	std::string functions;

	functions += "#define LIGHTING_ENABLE\n";

	return functions;
}

std::string ti::ShaderRegistry::SetLogic(int point_light, int dir_light) {
	std::string logic;

	for (int i = 0; i < point_light; i++)
		logic += "	Lo += PointCalculatePBR(N, V, point_light" + std::to_string(i) + ".position, point_light" + std::to_string(i) + ".color);\n";
	for (int i = 0; i < dir_light; i++)
		logic += "	Lo += DirCalculatePBR(N, V, dir_light" + std::to_string(i) + ".position, dir_light" + std::to_string(i) + ".direction, dir_light" + std::to_string(i) + ".color);\n";

	return logic;
}
