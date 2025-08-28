#ifndef GL_CONTEXT_H
#define GL_CONTEXT_H

#include <GLFW/glfw3.h>

int gl_context();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

extern GLFWwindow* window;
extern const int width;
extern const int height;

#endif
