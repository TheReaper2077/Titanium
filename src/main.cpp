#include "define.h"

#include "OpenGL/OpenGL.h"
#include "GLFWLayer.h"
#include "Camera.h"

#include "Math/math.h"

struct Vertex {
	Vec3 pos;
	Vec3 color;
};

int x = 0, y = 0, w = 1, h = 1;

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
	auto* uniformbuffer = UniformBuffer_Create();

	vertexbuffer->AddDataStatic(vertices.data(), vertices.size()*sizeof(Vertex));

	glm::mat4 proj = glm::ortho<float>(0, WIDTH, HEIGHT, 0, -100, 100);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);

	shader->BindUniformBlock("ProjectionMatrix", 0);

	uniformbuffer->BindRange(0, 3 * sizeof(glm::mat4));
	uniformbuffer->Allocate(3 * sizeof(glm::mat4));

	auto* camera = new Camera("cam0", PERSPECTIVE);

	uniformbuffer->AddDataDynamic(&camera->projection[0][0], sizeof(glm::mat4), 2 * sizeof(glm::mat4));
	uniformbuffer->AddDataDynamic(&camera->model[0][0], sizeof(glm::mat4), 0 * sizeof(glm::mat4));

	double dt = 0;
	while (!WindowExit()) {
		PollEvents();

		const auto& start_time = std::chrono::high_resolution_clock::now();
		// glEnable(GL_DEPTH_TEST);
		glClearColor(0.2, 0.2, 0.2, 0.2);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->Bind();
		vertexarray->Bind();
		vertexarray->BindVertexBuffer(vertexbuffer, vertexarray->stride);
		camera->Update(dt);
		
		uniformbuffer->AddDataDynamic(&camera->view[0][0], sizeof(glm::mat4), 1 * sizeof(glm::mat4));

		glDrawArrays(GL_TRIANGLES, 0, 6);

		SwapBuffers();

		const auto& end_time = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<double, std::ratio<1, 60>>(end_time - start_time).count();
	}
}