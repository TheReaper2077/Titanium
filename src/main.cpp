#include "define.h"

#include "OpenGL/OpenGL.h"
#include "GLFWLayer.h"

#include "Math/math.h"

struct Vertex {
	Vec3 pos;
	Vec3 color;
};

int x = 0, y = 0, w = 100, h = 100;

std::vector<Vertex> vertices = {
	{ Vec3(x, y, 0), Vec3(1, 1, 0) },
	{ Vec3(x + w, y, 0), Vec3(1, 1, 0) },
	{ Vec3(x + w, y + h, 0), Vec3(1, 1, 0) },
	{ Vec3(x + w, y + h, 0), Vec3(1, 1, 0) },
	{ Vec3(x, y + h, 0), Vec3(1, 1, 0) },
	{ Vec3(x, y, 0), Vec3(1, 1, 0) },
};

int main() {
	GLFW_CreateContext(WIDTH, HEIGHT, "2.5D Engine");
	OpenGL_CreateContext();

	glEnable(GL_LINE_SMOOTH);

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	auto* vertexarray = VertexArray_Create({ {0, 3, GL_FLOAT}, {1, 3, GL_FLOAT} });
	auto* vertexbuffer = VertexBuffer_Create();
	auto* shader = Shader_Create("D:\\C++\\2.5D Engine\\src\\Shaders\\default.vs", "D:\\C++\\2.5D Engine\\src\\Shaders\\ray.fs");

	vertexbuffer->AddDataStatic(vertices.data(), vertices.size()*sizeof(Vertex));

	glm::mat4 proj = glm::ortho<float>(0, WIDTH, HEIGHT, 0, -1, 1);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);

	auto* uniformbuffer = UniformBuffer_Create();

	shader->BindUniformBlock("ProjectionMatrix", 0);

	uniformbuffer->Allocate(3 * sizeof(glm::mat4));
	uniformbuffer->BindRange(0, 3 * sizeof(glm::mat4));
	uniformbuffer->AddDataDynamic(&proj[0][0], sizeof(glm::mat4), 2 * sizeof(glm::mat4));
	uniformbuffer->AddDataDynamic(&view[0][0], sizeof(glm::mat4), 1 * sizeof(glm::mat4));
	uniformbuffer->AddDataDynamic(&model[0][0], sizeof(glm::mat4), 0 * sizeof(glm::mat4));

	while (!WindowExit()) {
		PollEvents();

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2, 0.2, 0.2, 0.2);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->Bind();
		vertexarray->Bind();
		vertexbuffer->Bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);

		SwapBuffers();
	}
}