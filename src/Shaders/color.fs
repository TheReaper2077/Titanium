#version 400 core

in vec3 f_attr;
in vec3 f_normals;
in vec3 frag_pos;

// struct Material {
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
//     float shininess;
// };
  
// uniform Material material;

// uniform vec3 light_color;
// uniform vec3 light_pos;
// uniform vec3 view_pos;

void main() {
	// vec3 ambient = light_color * material.ambient;
  	
    // // diffuse 
    // vec3 norm = normalize(f_normals);
    // vec3 lightDir = normalize(light_pos - frag_pos);
    // float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = light_color * (diff * material.diffuse);
    
    // // specular
    // vec3 viewDir = normalize(view_pos - frag_pos);
    // vec3 reflectDir = reflect(-lightDir, norm);  
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 specular = light_color * (spec * material.specular);  
        
    // vec3 result = ambient + diffuse + specular + f_attr;

	// gl_FragColor = vec4(result, 1.0f);
	gl_FragColor = vec4(f_attr, 1.0f);
}