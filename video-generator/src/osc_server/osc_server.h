#ifndef OSC_SERVER_H
#define OSC_SERVER_H

#include <atomic>

int osc_server();

extern std::atomic<float> envelopeLeft;
extern std::atomic<float> param1Left;
extern std::atomic<float> param2Left;

extern std::atomic<float> envelopeRight;
extern std::atomic<float> param1Right;
extern std::atomic<float> param2Right;

#endif
