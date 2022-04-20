#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <assert.h>

#define GLFW_ASSERT(x) assert(x)
#define GLFW_API

struct GLFW_Context {
	GLFWwindow *window = nullptr;
	
	unsigned int key_state[GLFW_KEY_LAST + 1] = {GLFW_RELEASE};
	bool key_toggled[GLFW_KEY_LAST + 1] = {false};

	double xpos, ypos;
	unsigned int button_state[GLFW_MOUSE_BUTTON_LAST + 1] = {GLFW_RELEASE};
	bool button_toggled[GLFW_MOUSE_BUTTON_LAST + 1] = {false};
};

GLFW_API void GLFW_CreateContext(int width, int height, const char* title, int x = 50, int y = 50);
GLFW_API void GLFW_DeleteContext();

GLFW_API void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
GLFW_API bool IsKeyPressed(int key);
GLFW_API bool IsKeyReleased(int key);
GLFW_API bool IsKeyTyping(int key);
GLFW_API bool IsKeyToggled(int key);

GLFW_API void MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
GLFW_API void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
GLFW_API bool IsMousePressed(int button);
GLFW_API bool IsMouseReleased(int button);
GLFW_API bool IsMouseToggled(int button);
GLFW_API double GetMousePosX();
GLFW_API double GetMousePosY();

GLFW_API bool WindowExit();
GLFW_API void SwapBuffers();
GLFW_API void PollEvents();