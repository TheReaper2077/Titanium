#version 450 core

layout (location=0) in vec3 v_pos;
layout (location=1) in vec3 v_attr;
layout (location=2) in vec3 v_normal;

layout (std140, binding = 0) uniform ProjectionMatrix {
    mat4 model;
    mat4 view;
    mat4 projection;
	vec3 v_view_pos;
};

out vec3 f_uv;
out vec3 f_normals;
out vec3 frag_pos;
out vec3 view_pos;
	
void main() {
	f_uv = v_attr;
	f_normals = v_normal;

	frag_pos = vec3(model * vec4(v_pos, 1.0));
    // f_normals = mat3(transpose(inverse(model))) * v_normal;

	gl_Position = projection * view * vec4(frag_pos, 1.0);
	// gl_Position = vec4(v_pos, 1.0f);
	view_pos = v_view_pos;
}