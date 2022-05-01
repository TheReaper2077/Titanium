#version 450 core

// layout (location=0) in vec3 v_pos;
// layout (location=1) in vec3 v_attr;
// layout (location=2) in vec3 v_normal;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec3 uv0;
layout (location = 4) in vec3 uv1;
layout (location = 5) in vec3 uv2;
layout (location = 6) in vec3 uv3;
layout (location = 7) in vec3 uv4;
layout (location = 8) in vec3 uv5;
layout (location = 9) in vec3 uv6;
layout (location = 10) in vec3 uv7;

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
	f_uv = uv0;
	f_normals = normal;

	frag_pos = vec3(model * vec4(position, 1.0));
    // f_normals = mat3(transpose(inverse(model))) * normal;

	gl_Position = projection * view * vec4(frag_pos, 1.0);
	// gl_Position = vec4(position, 1.0f);
	view_pos = v_view_pos;
}