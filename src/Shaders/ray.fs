#version 400 core

in vec3 f_attr;
in vec3 f_normals;
in vec3 frag_pos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;       
};

struct AreaLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;   
};

uniform DirLight dir_light;
uniform PointLight point_light;
uniform SpotLight spot_light;
uniform AreaLight area_light;
uniform Material material;

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 view_dir) {
	vec3 light_dir = normalize(-light.direction);

	// diffuse shading
	float diff = max(dot(normal, light_dir), 0.0);
	// specular shading
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	vec3 ambient  = light.ambient * material.ambient;
    vec3 diffuse  = light.diffuse * (diff * material.diffuse);
    vec3 specular = light.specular * (spec * material.specular);
    return (ambient + diffuse + specular);
}

uniform vec3 view_pos;
uniform vec3 view_dir;

void main() {        
    vec3 result = CalculateDirLight(dir_light, f_normals, view_dir) + f_attr;
    gl_FragColor = vec4(result, 1.0);
}