#ifndef RENDER_LOOP_H
#define RENDER_LOOP_H

#include <vector>

void render_loop();

extern bool running;
extern std::chrono::steady_clock::time_point startTime;
extern unsigned int shaderProgram;
extern unsigned int VBO;
extern unsigned int VAO;
extern unsigned int EBO;
extern std::vector<unsigned char> buffer;
extern const int width;
extern const int height;

extern std::atomic<float> envelopeLeft;
extern std::atomic<float> param1Left;
extern std::atomic<float> param2Left;

extern std::atomic<float> envelopeRight;
extern std::atomic<float> param1Right;
extern std::atomic<float> param2Right;

#endif
