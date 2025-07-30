#ifndef GL_CONTEXT_H
#define GL_CONTEXT_H

#include <vector>
#include <GL/osmesa.h>

int gl_context();

extern std::vector<unsigned char> buffer;
extern OSMesaContext context;
extern const int width;
extern const int height;

#endif
