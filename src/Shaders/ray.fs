#version 400 core

in vec3 f_attr;
in vec3 f_normals;
in vec3 frag_pos;

void main() {
	gl_FragColor = vec4(f_attr, 1);
}