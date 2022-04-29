#version 400 core

in vec3 f_uv;
in vec3 f_normals;
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

uniform Material material;
uniform Light light;

vec3 CalculateDirLight(Material mat, Light light) {
	vec3 light_dir = normalize(-light.direction);

	vec3 ambient = light.ambient * (1 * mat.ambient);

    vec3 norm = normalize(f_normals);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * (diff * (texture(mat.diffuse_map, f_uv.xy).rgb * mat.diffuse));

	vec3 viewDir = normalize(view_pos - frag_pos);
    vec3 reflectDir = reflect(-light_dir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    vec3 specular = light.specular * (spec * (1 * mat.specular));

	return specular + ambient + diffuse;
}

vec3 CalculatePointLight(Material mat, Light light) {
	vec3 light_dir = normalize(light.position - frag_pos);

	vec3 ambient = light.ambient * (1 * mat.ambient);

    vec3 norm = normalize(f_normals);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * (diff * (texture(mat.diffuse_map, f_uv.xy).rgb * mat.diffuse));

	vec3 viewDir = normalize(view_pos - frag_pos);
    vec3 reflectDir = reflect(-light_dir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    vec3 specular = light.specular * (spec * (1 * mat.specular));

	float d = length(light.position - frag_pos);
	float attenuation = 1.0 / (light.constant + light.linear * d +  light.quadratic * (d * d));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return specular + ambient + diffuse;
}

void main() {
    // gl_FragColor = vec4(CalculatePointLight(material, light), 1.0f);
    gl_FragColor = vec4(CalculateDirLight(material, light), 1.0f);
}
