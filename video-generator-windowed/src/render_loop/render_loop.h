#ifndef RENDER_LOOP_H
#define RENDER_LOOP_H

#include <GLFW/glfw3.h>
#include <chrono>

void render_loop();
void processInput(GLFWwindow* window);

extern GLFWwindow* window;
extern std::chrono::steady_clock::time_point startTime;
extern unsigned int shaderProgram;
extern unsigned int VBO;
extern unsigned int VAO;
extern unsigned int EBO;
extern const int width;
extern const int height;

extern float attRelGenerator1;
extern float attRelGenerator2;

#endif
