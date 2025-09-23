#ifndef OSC_SERVER_H
#define OSC_SERVER_H

#include <atomic>

int osc_server();

// ===  TEST  ===
extern std::atomic<float> testLeft;
extern std::atomic<float> testRight;
// == == == == ==

#endif
