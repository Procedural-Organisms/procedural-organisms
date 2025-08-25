# include <iostream>
# include <atomic>
# include <chrono>

#include "osc_in_loop.h"

float attack = 0.01f;
float release = 0.2f;

float rampTime1 = 0.0f;
float attRelGenerator1 = 0.0f;
bool  attPase1 = false;

float rampTime2 = 0.0f;
float attRelGenerator2 = 0.0f;
bool  attPase2 = false;

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
        attPase1                                    == true
    ){ 
        if(msgSwitch1.load(std::memory_order_relaxed) == 1){

            std::cerr << "\n" << "Trigger 1" << std::endl;

            triggerTime1 = now;
            attRelGenerator1 = 0.0f;
            attPase1 = true;

            msgSwitch1.store(0, std::memory_order_relaxed);
        }
        elapsed1 = now - triggerTime1;
        float rampTime1 = elapsed1.count();

        if(rampTime1 / attack < 1.0f && attPase1 == true){
            attRelGenerator1 = rampTime1 / attack;
        }
        if(rampTime1 / attack > 1.0f && attPase1 == true){
            attPase1 = false;
            triggerTime1 = now;
            elapsed1 = now - triggerTime1;
            rampTime1 = elapsed1.count();
        }
        if(rampTime1 / -release + 1.0f > 0.0f && attPase1 == false){
            attRelGenerator1 = rampTime1 / -release + 1;
        }
        if(rampTime1 / -release + 1.0f < 0.0f && attPase1 == false){
            attRelGenerator1 = 0.0f;
        }
        std::cerr << attRelGenerator1 << std::endl;
    }



    if(
        msgSwitch2.load(std::memory_order_relaxed)  == 1    ||
        attRelGenerator2                            != 0.0f ||
        attPase2                                    == true
    ){ 
        if(msgSwitch2.load(std::memory_order_relaxed) == 1){

            std::cerr << "\n" << "Trigger 2" << std::endl;

            triggerTime2 = now;
            attRelGenerator2 = 0.0f;
            attPase2 = true;

            msgSwitch2.store(0, std::memory_order_relaxed);
        }
        elapsed2 = now - triggerTime2;
        float rampTime2 = elapsed2.count();

        if(rampTime2 / attack < 1.0f && attPase2 == true){
            attRelGenerator2 = rampTime2 / attack;
        }
        if(rampTime2 / attack > 1.0f && attPase2 == true){
            attPase2 = false;
            triggerTime2 = now;
            elapsed2 = now - triggerTime2;
            rampTime2 = elapsed2.count();
        }
        if(rampTime2 / -release + 1.0f > 0.0f && attPase2 == false){
            attRelGenerator2 = rampTime2 / -release + 1;
        }
        if(rampTime2 / -release + 1.0f < 0.0f && attPase2 == false){
            attRelGenerator2 = 0.0f;
        }
        std::cerr << attRelGenerator2 << std::endl;
    }
}
