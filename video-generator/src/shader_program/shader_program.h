#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H 

#include <GL/osmesa.h>

std::string loadShaderSource(const char* shaderPath);
int shader_program();

extern OSMesaContext context;
extern unsigned int shaderProgram;

#endif

