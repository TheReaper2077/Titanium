#version 400 core

in vec3 f_attr;

uniform sampler2DArray u_texturearray;

void main() {
	gl_FragColor = texture(u_texturearray, f_attr) * vec4(1);
}
