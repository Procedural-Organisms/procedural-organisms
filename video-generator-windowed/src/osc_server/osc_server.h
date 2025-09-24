#ifndef OSC_SERVER_H
#define OSC_SERVER_H

#include <atomic>

int osc_server();

// ===  TEST  ===
extern std::atomic<float> envelopeLeft;
extern std::atomic<float> param1Left;
extern std::atomic<float> param2Left;

extern std::atomic<float> testRight;
// == == == == ==

#endif
