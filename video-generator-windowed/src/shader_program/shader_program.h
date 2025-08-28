#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H 

#include <glad/glad.h>
#include <string>

std::string loadShaderSource(const char* shaderPath);
int shader_program();

extern unsigned int shaderProgram;

#endif

