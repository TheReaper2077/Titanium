#include <OpenGL.h>

#ifndef SHADER_TEXTUREARRAY
#define SHADER_TEXTUREARRAY "u_texturearray"
#endif

#ifndef SHADER_TEXTURES
#define SHADER_TEXTURES "u_textures"
#endif

#ifndef SHADER_LIGHT
#define SHADER_LIGHT
#endif

#include <fstream>
#include <iostream>

static std::string read_file(const std::string &filename) {
	std::string line, text;
	std::fstream file(filename);

	while (std::getline(file, line)) text += (line + '\n');

	file.close();

	return text;
}

extern std::shared_ptr<OpenGLContext> gl_context;

void Compile(unsigned int &program, const std::string &filename, unsigned int target, bool file) {
	unsigned int shader = glCreateShader(target);
	std::string text;
	if (file) text = read_file(filename);
	else text = filename;
	const char* src = text.c_str();
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	unsigned int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, (GLint*)&result);

	if (!result) {
		unsigned int logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, (GLint*)&logLength);
		GLchar* message = (GLchar*)alloca(logLength*sizeof(char));
		glGetShaderInfoLog(shader, logLength, (GLsizei*)&logLength, message);
		std::cout << message << std::endl;
	}

	GL_ASSERT(result);

	glAttachShader(program, shader);
	glDeleteShader(shader);
};

Shader *Shader_Create(std::string name, const std::string &vs_filename, const std::string &fs_filename, bool file) {
	GL_ASSERT(gl_context != nullptr);

	unsigned int program = glCreateProgram();

	Compile(program, vs_filename, GL_VERTEX_SHADER, file);
	Compile(program, fs_filename, GL_FRAGMENT_SHADER, file);

	glLinkProgram(program);

	auto shader = std::make_shared<Shader>();

	shader->id = program;
	shader->name = name;

	gl_context->shader_store.push_back(shader);
	
	auto* shader_ptr = shader.get();

	shader_ptr->Bind();

	if (shader_ptr->GetUniformLoc(SHADER_TEXTUREARRAY)) {
		shader_ptr->texture_array = true;

		// for (int i = 0; i < 32; i++) {
		// 	glActiveTexture(GL_TEXTURE0 + i);
		// 	auto tmp = std::string(SHADER_TEXTUREARRAY) + std::to_string(i);
		// 	Shader_SetUniformi(shader_ptr, tmp, i);
		// }
		glActiveTexture(GL_TEXTURE0);
		shader_ptr->SetUniformi(SHADER_TEXTUREARRAY, 0);
	}
	if (shader_ptr->GetUniformLoc(SHADER_TEXTURES)) {
		shader_ptr->textures = true;
		
		for (int i = 0; i < 32; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			auto tmp = std::string(SHADER_TEXTURES) + std::to_string(i);
			shader_ptr->SetUniformi(tmp, i);
		}
	}
	if (shader_ptr->GetUniformLoc("light")) {
		shader_ptr->light = true;
	}
	if (shader_ptr->GetUniformLoc("material")) {
		shader_ptr->material = true;
	}

	return shader_ptr;
}

void Shader::Bind() {
	if (gl_context->binding_shader == this->id) return;
	gl_context->binding_shader = this->id;
	glUseProgram(this->id);
}

void Shader::UnBind() {
	gl_context->binding_vertexarray = 0;
	glUseProgram(0);
}

void Shader::BindUniformBlock(std::string name, unsigned int index) {
	glUniformBlockBinding(this->id, glGetUniformBlockIndex(this->id, name.c_str()), index);
}

unsigned int Shader::GetUniformLoc(std::string name) {
	if (this->uniform_location_map.find(name) == this->uniform_location_map.end())
		this->uniform_location_map[name] = glGetUniformLocation(this->id, name.c_str());
		
	return this->uniform_location_map[name];
}

void Shader::SetUniformMat4(std::string uniform, const float* matrix) {
	this->Bind();
	glUniformMatrix4fv(this->GetUniformLoc(uniform), 1, GL_FALSE, matrix);
}

void Shader::SetUniformi(std::string uniform, int v0) {
	this->Bind();
	glUniform1i(this->GetUniformLoc(uniform), v0);
}

void Shader::SetUniformf(std::string uniform, float v0) {
	this->Bind();
	glUniform1f(this->GetUniformLoc(uniform), v0);
}

void Shader::SetUniformVec3(std::string uniform, const float *v) {
	this->Bind();
	glUniform3fv(this->GetUniformLoc(uniform), 1, v);
}

void Shader::SetUniformVec4(std::string uniform, const float *v) {
	this->Bind();
	glUniform4fv(this->GetUniformLoc(uniform), 1, v);
}

void Shader::SetUniformArray(std::string uniform, std::size_t count, const float *v) {
	this->Bind();
	glUniform1fv(this->GetUniformLoc(uniform), count, v);
}
