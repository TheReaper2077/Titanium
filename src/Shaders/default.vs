#version 450 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec4 v_color;
layout (location = 3) in vec2 v_uv0;
layout (location = 4) in vec2 v_uv1;
layout (location = 5) in vec2 v_uv2;
layout (location = 6) in vec2 v_uv3;
layout (location = 7) in vec2 v_uv4;
layout (location = 8) in vec2 v_uv5;
layout (location = 9) in vec2 v_uv6;
layout (location = 10) in vec2 v_uv7;

layout (std140, binding = 0) uniform ProjectionMatrix {
    mat4 model;
    mat4 view;
    mat4 projection;
	vec3 v_view_pos;
};

out vec3 normal;
out vec4 color;
out vec2 uv0;
out vec2 uv1;
out vec2 uv2;
out vec2 uv3;
out vec2 uv4;
out vec2 uv5;
out vec2 uv6;
out vec2 uv7;

out vec3 frag_pos;
out vec3 view_pos;
	
void main() {
	normal = normalize(v_normal);
	color = v_color;
	uv0 = v_uv0;
	uv1 = v_uv1;
	uv2 = v_uv2;
	uv3 = v_uv3;
	uv4 = v_uv4;
	uv5 = v_uv5;
	uv6 = v_uv6;
	uv7 = v_uv7;

	frag_pos = vec3(model * vec4(v_position, 1.0));
	view_pos = v_view_pos;

	gl_Position = projection * view * vec4(frag_pos, 1.0);
}