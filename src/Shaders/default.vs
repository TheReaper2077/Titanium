#version 450 core

layout (location=0) in vec3 v_pos;
layout (location=1) in vec3 v_attr;
layout (location=2) in vec3 v_normals;

layout (std140) uniform ProjectionMatrix {
    mat4 model;
    mat4 view;
    mat4 projection;
};

out vec3 f_attr;
out vec3 f_normals;
out vec3 frag_pos;
	
uniform vec3 offset;

void main() {
	f_attr = v_attr;
	f_normals = v_normals;

	frag_pos = vec3(model * vec4(v_pos + offset, 1.0));
    // f_normals = mat3(transpose(inverse(model))) * v_normals;

	gl_Position = projection * view * vec4(frag_pos, 1.0);
	// gl_Position = vec4(v_pos, 1.0f);
}