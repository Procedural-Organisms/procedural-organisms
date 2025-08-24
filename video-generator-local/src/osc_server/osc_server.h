#ifndef OSC_SERVER_H
#define OSC_SERVER_H

#include <atomic>

int osc_server();

extern std::atomic<int> msgSwitch1;
extern std::atomic<int> msgSwitch2;

#endif
