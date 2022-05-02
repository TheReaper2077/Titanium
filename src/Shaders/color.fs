#version 400 core

in vec3 normal;
in vec4 color;
in vec2 uv0;
in vec2 uv1;
in vec2 uv2;
in vec2 uv3;
in vec2 uv4;
in vec2 uv5;
in vec2 uv6;
in vec2 uv7;

in vec3 frag_pos;
in vec3 view_pos;

struct Material {
    vec3 ambient;
	vec3 diffuse;
    vec3 specular;

    float shininess;
	float roughness;

	sampler2D ambient_map;
    sampler2D diffuse_map;
	sampler2D specular_map;
};

struct Light {
	vec3 position;  
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
	
	float cutOff;
	float outerCutOff;
};

vec3 CalculateDirLight(Material mat, Light light, vec3 normal, vec3 view_dir) {
	vec3 light_dir = normalize(-light.direction);
    float diff = max(dot(normal, light_dir), 0.0);
    float spec = max(dot(view_dir, reflect(-light_dir, normal)), 0.0);

	vec3 ambient = light.ambient * (1 * mat.ambient);
	vec3 diffuse = light.diffuse * (diff * (1 * mat.diffuse));
    vec3 specular = light.specular * (pow(spec, mat.shininess) * (1 * mat.specular));

	return specular + ambient + diffuse;
}

vec3 CalculatePointLight(Material mat, Light light, vec3 normal, vec3 view_dir) {
	vec3 light_dir = normalize(light.position - frag_pos);
    float diff = max(dot(normal, light_dir), 0.0);
    float spec = max(dot(view_dir, reflect(-light_dir, normal)), 0.0);

	vec3 ambient = light.ambient * (1 * mat.ambient);
	vec3 diffuse = light.diffuse * (diff * (1 * mat.diffuse));
    vec3 specular = light.specular * (pow(spec, mat.shininess) * (1 * mat.specular));

	float d = length(light.position - frag_pos);
	float attenuation = 1.0 / (light.constant + light.linear * d +  light.quadratic * (d * d));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return specular + ambient + diffuse;
}

uniform Material material;
uniform Light light;

void main() {
	vec3 view_dir = normalize(view_pos - frag_pos);

	vec3 result;
	
	result = CalculateDirLight(material, light, normal, view_dir);

	gl_FragColor = vec4(result, 1);
}
