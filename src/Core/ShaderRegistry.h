#pragma once

#include "Components/Components.h"
#include <OpenGL.h>
#include "ECS.h"
#include <string>

// #define CALC_AREA_LIGHT "CalcAreaLight"
// #define CALC_SPOT_LIGHT \
// // float theta = dot(lightDir, normalize(-light.direction));
// "vec3 CalculatePointLight(Material mat, Light light, vec3 normal, vec3 view_dir) {\n" \
// "	vec3 light_dir = normalize(light.position - frag_pos);\n" \
// "\n" \
// "	float theta = dot(light_dir, normalize(-light.direction));\n" \
// "	if (theta > light.cutOff) {\n" \
// "		vec3 ambient;\n" \
// "		float diff = max(dot(normal, light_dir), 0.0);\n" \
// "		float spec = max(dot(view_dir, reflect(-light_dir, normal)), 0.0);\n" \
// "		vec3 diffuse;\n" \
// "		vec3 specular;\n" \
// "		if (ambient_index > 0)\n" \
// "			ambient = light.ambient * texture(mat.ambient_map, uv0).rgb * mat.ambient;\n" \
// "		else\n" \
// "			ambient = light.ambient * mat.ambient;\n" \
// "		if (diffuse_index > 0)\n" \
// "			diffuse = light.diffuse * diff * texture(mat.diffuse_map, uv0).rgb * mat.diffuse;\n" \
// "		else\n" \
// "			diffuse = light.diffuse * diff * mat.diffuse;\n" \
// "		if (specular_index > 0)\n" \
// "			specular = light.specular * spec * texture(mat.specular_map, uv0).rgb * mat.specular;\n" \
// "		else\n" \
// "			specular = light.specular * spec * mat.specular;\n" \
// "\n" \
// "		float distance = length(light.position - frag_pos);\n" \
// "		float attenuation = 1.0f / float(light.constant + light.linear * distance + light.quadratic * (distance * distance));\n" \
// "\n" \
// "		diffuse *= attenuation;\n" \
// "		specular *= attenuation;\n" \
// "		return specular + ambient + diffuse;\n" \
// "	}\n" \
// "\n" \
// "	if (ambient_index > 0)\n" \
// "		return light.ambient * texture(mat.ambient_map, uv0).rgb * mat.ambient;\n" \
// "	else\n" \
// "		return light.ambient * mat.ambient;\n" \
// "}\n"

#define CALC_POINT_LIGHT \
"vec3 CalculatePointLight(Material mat, Light light, vec3 normal, vec3 view_dir) {\n" \
"	vec3 light_dir = normalize(light.position - frag_pos);\n" \
"	float diff = max(dot(normal, light_dir), 0.0);\n" \
"	float spec = max(dot(view_dir, reflect(-light_dir, normal)), 0.0);\n" \
"\n" \
"	vec3 ambient;\n" \
"	vec3 diffuse;\n" \
"	vec3 specular;\n" \
"	if (ambient_index > 0)\n" \
"		ambient = light.ambient * texture(mat.ambient_map, uv0).rgb * mat.ambient;\n" \
"	else\n" \
"		ambient = light.ambient * mat.ambient;\n" \
"	if (diffuse_index > 0)\n" \
"		diffuse = light.diffuse * diff * texture(mat.diffuse_map, uv0).rgb * mat.diffuse;\n" \
"	else\n" \
"		diffuse = light.diffuse * diff * mat.diffuse;\n" \
"	if (specular_index > 0)\n" \
"		specular = light.specular * spec * texture(mat.specular_map, uv0).rgb * mat.specular;\n" \
"	else\n" \
"		specular = light.specular * spec * mat.specular;\n" \
"\n" \
"	float distance = length(light.position - frag_pos);\n" \
"	float attenuation = 1.0f / float(light.constant + light.linear * distance + light.quadratic * (distance * distance));\n" \
"\n" \
"	ambient *= attenuation;\n" \
"	diffuse *= attenuation;\n" \
"	specular *= attenuation;\n" \
"\n" \
"	return specular + ambient + diffuse;\n" \
"}\n"

#define CALC_DIRECTIONAL_LIGHT \
"vec3 CalculateDirectionalLight(Material mat, Light light, vec3 normal, vec3 view_dir) {\n" \
"	vec3 light_dir = normalize(-light.direction);\n" \
"	float diff = max(dot(normal, light_dir), 0.0);\n" \
"	float spec = pow(max(dot(view_dir, reflect(-light_dir, normal)), 0.0), mat.shininess);\n" \
"\n" \
"	vec3 ambient;\n" \
"	vec3 diffuse;\n" \
"	vec3 specular;\n" \
"	if (ambient_index > 0)\n" \
"		ambient = light.ambient * texture(mat.ambient_map, uv0).rgb * mat.ambient;\n" \
"	else\n" \
"		ambient = light.ambient * mat.ambient;\n" \
"	if (diffuse_index > 0)\n" \
"		diffuse = light.diffuse * diff * texture(mat.diffuse_map, uv0).rgb * mat.diffuse;\n" \
"	else\n" \
"		diffuse = light.diffuse * diff * mat.diffuse;\n" \
"	if (specular_index > 0)\n" \
"		specular = light.specular * spec * texture(mat.specular_map, uv0).rgb * mat.specular;\n" \
"	else\n" \
"		specular = light.specular * spec * mat.specular;\n" \
"\n" \
"	return specular + ambient + diffuse;\n" \
"}\n"

#define VS_SHADER_MAIN \
"#version 400 core\n" \
"\n" \
"layout (location = 0) in vec3 v_position;\n" \
"layout (location = 1) in vec3 v_normal;\n" \
"layout (location = 2) in vec4 v_color;\n" \
"layout (location = 3) in vec2 v_uv0;\n" \
"layout (location = 4) in vec2 v_uv1;\n" \
"layout (location = 5) in vec2 v_uv2;\n" \
"layout (location = 6) in vec2 v_uv3;\n" \
"layout (location = 7) in vec2 v_uv4;\n" \
"layout (location = 8) in vec2 v_uv5;\n" \
"layout (location = 9) in vec2 v_uv6;\n" \
"layout (location = 10) in vec2 v_uv7;\n" \
"\n" \
"layout (std140, binding = 0) uniform ProjectionMatrix {\n" \
"	mat4 model;\n" \
"	mat4 view;\n" \
"	mat4 projection;\n" \
"	vec3 v_view_pos;\n" \
"};\n" \
"\n" \
"out vec3 normal;\n" \
"out vec4 color;\n" \
"out vec2 uv0;\n" \
"out vec2 uv1;\n" \
"out vec2 uv2;\n" \
"out vec2 uv3;\n" \
"out vec2 uv4;\n" \
"out vec2 uv5;\n" \
"out vec2 uv6;\n" \
"out vec2 uv7;\n" \
"\n" \
"out vec3 frag_pos;\n" \
"out vec3 view_pos;\n" \
"\n" \
"void main() {\n" \
"	normal = mat3(transpose(inverse(model))) * v_normal;\n" \
"	color = v_color;\n" \
"	uv0 = v_uv0;\n" \
"	uv1 = v_uv1;\n" \
"	uv2 = v_uv2;\n" \
"	uv3 = v_uv3;\n" \
"	uv4 = v_uv4;\n" \
"	uv5 = v_uv5;\n" \
"	uv6 = v_uv6;\n" \
"	uv7 = v_uv7;\n" \
"\n" \
"	frag_pos = vec3(model * vec4(v_position, 1.0));\n" \
"	view_pos = v_view_pos;\n" \
"\n" \
"	gl_Position = projection * view * vec4(frag_pos, 1.0);\n" \
"}"

#define FS_INITIALISE \
"#version 400 core\n" \
"\n" \
"in vec3 normal;\n" \
"in vec4 color;\n" \
"in vec2 uv0;\n" \
"in vec2 uv1;\n" \
"in vec2 uv2;\n" \
"in vec2 uv3;\n" \
"in vec2 uv4;\n" \
"in vec2 uv5;\n" \
"in vec2 uv6;\n" \
"in vec2 uv7;\n" \
"\n" \
"in vec3 frag_pos;\n" \
"in vec3 view_pos;\n" \
"\n" \
"struct Material {\n" \
"	vec3 ambient;\n" \
"	vec3 diffuse;\n" \
"	vec3 specular;\n" \
"\n" \
"	float shininess;\n" \
"	float roughness;\n" \
"\n" \
"	sampler2D ambient_map;\n" \
"	sampler2D diffuse_map;\n" \
"	sampler2D specular_map;\n" \
"};\n" \
"\n" \
"struct Light {\n" \
"	vec3 position;  \n" \
"	vec3 direction;\n" \
"\n" \
"	vec3 ambient;\n" \
"	vec3 diffuse;\n" \
"	vec3 specular;\n" \
"\n" \
"	float constant;\n" \
"	float linear;\n" \
"	float quadratic;\n" \
"	\n" \
"	float cutOff;\n" \
"	float outerCutOff;\n" \
"};\n" \
"\n" \
"uniform Material material;\n" \
"\n" \
"uniform int ambient_index;\n" \
"uniform int diffuse_index;\n" \
"uniform int specular_index;\n" \
"\n" \
"uniform int has_texture;\n" \
"\n"

#define HAS_MATERIALS 0x01
#define HAS_TEXTURE 0x02
#define HAS_COLOR 0x04

namespace ti {
	struct ShaderRegistry {
		std::unordered_map<std::string, Shader*> registry;

		Shader* GetShader(ti::ECS::Registry* registry) {
			using namespace ti::Component;

			int point_light = 0;
			int dir_light = 0;
			int spot_light = 0;
			int area_light = 0;

			for (auto& entity: registry->View<Tag, Transform, Light>()) {
				auto& light = registry->Get<Light>(entity);

				if (!light.active) continue;

				if (light.active) {
					if (light.mode == Point) point_light++;
					if (light.mode == Spot) spot_light++;
					if (light.mode == Area) area_light++;
					if (light.mode == Directional) dir_light++;
				}
			}

			return RegisterShader(0, point_light, dir_light, spot_light, area_light);
		}

		void AddShader(Shader* shader) {
			registry[shader->name] = shader;
		}

		Shader* GetShader(std::string name) {
			return registry[name];
		}

		Shader* RegisterShader(uint32_t shader_flags, int point_light, int dir_light, int spot_light, int area_light) {
			std::string id = std::to_string(shader_flags) + '_' + std::to_string(dir_light) + '_' + std::to_string(point_light) + '_' + std::to_string(spot_light) + '_' + std::to_string(area_light);
	
			if (registry.find(id) == registry.end()) {
				std::string fragmentshader = FS_INITIALISE;

				fragmentshader += SetUniforms(point_light, dir_light, spot_light, area_light);
				fragmentshader += SetFunctions(point_light, dir_light, spot_light, area_light);

				fragmentshader += "\nvoid main() {\n";
				fragmentshader += "	if (has_texture > 0) {\n		gl_FragData[0] = color;\n		return;\n	}";

				fragmentshader += SetLogic(point_light, dir_light, spot_light, area_light);

				fragmentshader += "	gl_FragData[0] = vec4(result, 1.0);\n}\n";
				// fragmentshader += "	gl_FragColor = vec4(result, 1.0);\n}\n";

				// std::cout << fragmentshader << '\n' << id << '\n';

				registry[id] = Shader_Create(id, VS_SHADER_MAIN, fragmentshader, false);
			}

			return registry[id];
		}

		std::string SetUniforms(int point_light, int dir_light, int spot_light, int area_light) {
			std::string uniforms;

			for (int i = 0; i < point_light; i++) {
				uniforms += "uniform Light point" + std::to_string(i) + ";\n";
			}
			for (int i = 0; i < dir_light; i++) {
				uniforms += "uniform Light directional" + std::to_string(i) + ";\n";
			}
			for (int i = 0; i < spot_light; i++) {
				uniforms += "uniform Light spot" + std::to_string(i) + ";\n";
			}
			for (int i = 0; i < area_light; i++) {
				uniforms += "uniform Light area" + std::to_string(i) + ";\n";
			}

			uniforms += "\n";

			return uniforms;
		}

		std::string SetFunctions(int point_light, int dir_light, int spot_light, int area_light) {
			std::string functions;

			if (dir_light) {
				functions += CALC_DIRECTIONAL_LIGHT;
			}
			if (point_light) {
				functions += CALC_POINT_LIGHT;
			}
			if (spot_light) {
				// functions += CALC_SPOT_LIGHT;
			}

			functions += "\n";

			return functions;
		}

		std::string SetLogic(int point_light, int dir_light, int spot_light, int area_light) {
			std::string logic = "	vec3 view_dir = normalize(view_pos - frag_pos);\n	vec3 result;\n\n";

			for (int i = 0; i < dir_light; i++) {
				logic += "	result += CalculateDirectionalLight(material, directional" + std::to_string(i) + ", normal, view_dir);\n";
			}

			for (int i = 0; i < point_light; i++) {
				logic += "	result += CalculatePointLight(material, point" + std::to_string(i) + ", normal, view_dir);\n";
			}
			
			// for (int i = 0; i < spot_light; i++) {
			// 	logic += "	result += CalculateSpotLight(material, spot" + std::to_string(i) + ", normal, view_dir);\n";
			// }
			// for (int i = 0; i < area_light; i++) {
			// 	logic += "result += CalculatePointLight(material, point" + std::to_string(i) + ", normal, view_dir);\n";
			// }

			logic += "\n";

			return logic;
		}
	};
}
