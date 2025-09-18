#ifndef OSC_SERVER_H
#define OSC_SERVER_H

#include <atomic>

int osc_server();

extern std::atomic<int> msgSwitch1;
extern std::atomic<int> msgSwitch2;
extern std::atomic<float> attack1;
extern std::atomic<float> release1;
extern std::atomic<float> attack2;
extern std::atomic<float> release2;

// ===  TEST  ===
extern std::atomic<float> test;
// == == == == ==

#endif
