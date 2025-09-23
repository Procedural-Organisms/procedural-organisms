# include <iostream>
# include <atomic>
# include <chrono>

#include "osc_in_loop.h"

auto now = std::chrono::steady_clock::now();

std::chrono::steady_clock::time_point triggerTime1 = now;
std::chrono::duration<float> elapsed1 = now - triggerTime1;

std::chrono::steady_clock::time_point triggerTime2 = now;
std::chrono::duration<float> elapsed2 = now - triggerTime2;

// TODO crear objeto osc_in_loop para poder llamar varias instancias
// dentro de distintas funciones dentro del loop
std::atomic<float> osc_in_loop(int functionIndex = 0){
    // TODO crear clase AttackReleaseGenerator

    now = std::chrono::steady_clock::now();

    // ===  TEST  ===  
    if(functionIndex == 0){
        return testLeft.load(std::memory_order_relaxed);
    }
    if(functionIndex == 1){
        return testRight.load(std::memory_order_relaxed);
    }
    else{
        return -1;
    }
    // == == == == ==
}
