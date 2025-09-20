#ifndef OSC_IN_LOOP_H
#define OSC_IN_LOOP_H

#include <atomic>

std::atomic<float> osc_in_loop();

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
