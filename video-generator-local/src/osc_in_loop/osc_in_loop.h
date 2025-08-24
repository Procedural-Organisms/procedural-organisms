#ifndef OSC_IN_LOOP_H
#define OSC_IN_LOOP_H

#include <atomic>

void osc_in_loop();

extern std::atomic<int> msgSwitch1;
extern std::atomic<int> msgSwitch2;

#endif
