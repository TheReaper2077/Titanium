#include "GLFWLayer.h"

static std::shared_ptr<GLFW_Context> glfw_context;

void GLFW_CreateContext(int width, int height, const char* title, int x, int y) {
	GLFW_ASSERT(glfw_context == nullptr);

	glfw_context = std::make_shared<GLFW_Context>();

	GLFW_ASSERT(glfwInit());

	glfw_context->window = glfwCreateWindow(width, height, title, NULL, NULL);
	glfwMakeContextCurrent(glfw_context->window);
	
	GLFW_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

	glfwSetWindowPos(glfw_context->window, x, y);
	
	glfwSetKeyCallback(glfw_context->window, KeyCallback);
	glfwSetCursorPosCallback(glfw_context->window, MouseCursorPosCallback);
	glfwSetMouseButtonCallback(glfw_context->window, MouseButtonCallback);
}

void GLFW_DeleteContext() {
	GLFW_ASSERT(glfw_context != nullptr);

	glfwDestroyWindow(glfw_context->window);
	glfwTerminate();
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	GLFW_ASSERT(glfw_context != nullptr);

	if (glfw_context->key_state[key] == GLFW_RELEASE && action == GLFW_PRESS) {
		glfw_context->key_toggled[key] = !glfw_context->key_toggled[key];
	}
	glfw_context->key_state[key] = action;
}

bool IsKeyPressed(int key) {
	GLFW_ASSERT(glfw_context != nullptr);

	return (glfw_context->key_state[key] == GLFW_PRESS || glfw_context->key_state[key] == GLFW_REPEAT);
}

bool IsKeyReleased(int key) {
	GLFW_ASSERT(glfw_context != nullptr);

	return (glfw_context->key_state[key] == GLFW_RELEASE);
}

bool IsKeyTyping(int key) {
	GLFW_ASSERT(glfw_context != nullptr);

	return (glfw_context->key_state[key] == GLFW_REPEAT);
}

bool IsKeyToggled(int key) {
	GLFW_ASSERT(glfw_context != nullptr);

	return glfw_context->key_toggled[key];
}

void MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	GLFW_ASSERT(glfw_context != nullptr);

	glfw_context->xpos = xpos;
	glfw_context->ypos = ypos;
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	GLFW_ASSERT(glfw_context != nullptr);

	if (glfw_context->button_state[button] == GLFW_RELEASE && action == GLFW_PRESS) {
		glfw_context->button_toggled[button] = !glfw_context->button_toggled[button];
	}
	glfw_context->button_state[button] = action;
}

bool IsMousePressed(int button) {
	GLFW_ASSERT(glfw_context != nullptr);

	return (glfw_context->button_state[button] == GLFW_PRESS);
}

bool IsMouseReleased(int button) {
	GLFW_ASSERT(glfw_context != nullptr);

	return (glfw_context->button_state[button] == GLFW_RELEASE);
}

bool IsMouseToggled(int button) {
	GLFW_ASSERT(glfw_context != nullptr);

	return (glfw_context->button_toggled[button]);
}

double GetMousePosX() {
	GLFW_ASSERT(glfw_context != nullptr);

	return glfw_context->xpos;
}

double GetMousePosY() {
	GLFW_ASSERT(glfw_context != nullptr);

	return glfw_context->ypos;
}

bool WindowExit() {
	GLFW_ASSERT(glfw_context != nullptr);

	return (glfwWindowShouldClose(glfw_context->window));
}

void SwapBuffers() {
	GLFW_ASSERT(glfw_context != nullptr);

	glfwSwapBuffers(glfw_context->window);
}

void PollEvents() {
	GLFW_ASSERT(glfw_context != nullptr);

	glfwPollEvents();
}