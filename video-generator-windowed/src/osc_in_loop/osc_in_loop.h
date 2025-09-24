#ifndef OSC_IN_LOOP_H
#define OSC_IN_LOOP_H

#include <atomic>

std::atomic<float> osc_in_loop(int functionIndex);

// ===  TEST  ===  
extern std::atomic<float> envelopeLeft;
extern std::atomic<float> testRight;
// == == == == ==

#endif
