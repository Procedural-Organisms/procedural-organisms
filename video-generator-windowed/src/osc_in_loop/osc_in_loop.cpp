# include <iostream>
# include <atomic>
# include <chrono>

#include "osc_in_loop.h"

float rampTime1 = 0.0f;
float attRelGenerator1 = 0.0f;
bool  attPhase1 = false;

float rampTime2 = 0.0f;
float attRelGenerator2 = 0.0f;
bool  attPhase2 = false;

auto now = std::chrono::steady_clock::now();

std::chrono::steady_clock::time_point triggerTime1 = now;
std::chrono::duration<float> elapsed1 = now - triggerTime1;

std::chrono::steady_clock::time_point triggerTime2 = now;
std::chrono::duration<float> elapsed2 = now - triggerTime2;

void osc_in_loop(){
    // TODO crear clase AttackReleaseGenerator

    now = std::chrono::steady_clock::now();
    
    // cuando msgSwitch cambia a 1 la funcion exchange se ejecuta
    // ejecutando la funcion dentro de la condicional
    // y cambiando inmediatamente el valor de msgSwitch de vuelta a 0
    if(
        msgSwitch1.load(std::memory_order_relaxed)  == 1    ||
        attRelGenerator1                            != 0.0f ||
        attPhase1                                   == true
    ){ 
        if(msgSwitch1.load(std::memory_order_relaxed) == 1){
            triggerTime1 = now;
            attRelGenerator1 = 0.0f;
            attPhase1 = true;

            msgSwitch1.store(0, std::memory_order_relaxed);
        }
        elapsed1 = now - triggerTime1;
        float rampTime1 = elapsed1.count();

        if(rampTime1 / attack1.load(std::memory_order_relaxed) < 1.0f && attPhase1 == true){
            attRelGenerator1 = rampTime1 / attack1.load(std::memory_order_relaxed);
        }
        if(rampTime1 / attack1.load(std::memory_order_relaxed) > 1.0f && attPhase1 == true){
            attPhase1 = false;
            triggerTime1 = now;
            elapsed1 = now - triggerTime1;
            rampTime1 = elapsed1.count();
        }
        if(rampTime1 / -release1.load(std::memory_order_relaxed) + 1.0f > 0.0f && attPhase1 == false){
            attRelGenerator1 = rampTime1 / -release1.load(std::memory_order_relaxed) + 1;
        }
        if(rampTime1 / -release1.load(std::memory_order_relaxed) + 1.0f < 0.0f && attPhase1 == false){
            attRelGenerator1 = 0.0f;
        }
    }



    if(
        msgSwitch2.load(std::memory_order_relaxed)  == 1    ||
        attRelGenerator2                            != 0.0f ||
        attPhase2                                   == true
    ){ 
        if(msgSwitch2.load(std::memory_order_relaxed) == 1){
            triggerTime2 = now;
            attRelGenerator2 = 0.0f;
            attPhase2 = true;

            msgSwitch2.store(0, std::memory_order_relaxed);
        }
        elapsed2 = now - triggerTime2;
        float rampTime2 = elapsed2.count();

        if(rampTime2 / attack2.load(std::memory_order_relaxed) < 1.0f && attPhase2 == true){
            attRelGenerator2 = rampTime2 / attack2.load(std::memory_order_relaxed);
        }
        if(rampTime2 / attack2.load(std::memory_order_relaxed) > 1.0f && attPhase2 == true){
            attPhase2 = false;
            triggerTime2 = now;
            elapsed2 = now - triggerTime2;
            rampTime2 = elapsed2.count();
        }
        if(rampTime2 / -release2.load(std::memory_order_relaxed) + 1.0f > 0.0f && attPhase2 == false){
            attRelGenerator2 = rampTime2 / -release2.load(std::memory_order_relaxed) + 1;
        }
        if(rampTime2 / -release2.load(std::memory_order_relaxed) + 1.0f < 0.0f && attPhase2 == false){
            attRelGenerator2 = 0.0f;
        }
    }
}
