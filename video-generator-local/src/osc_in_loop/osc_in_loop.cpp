# include <iostream>
# include <atomic>

#include "osc_in_loop.h"

void osc_in_loop(){
    // cuando msgSwitch cambia a 1 la funcion exchange se ejecuta
    // ejecutando la funcion dentro de la condicional
    // y cambiando inmediatamente el valor de msgSwitch de vuelta a 0
    if(msgSwitch1.exchange(0, std::memory_order_relaxed)){
        std::cerr << "Trigger 1" << std::endl;
    }

    if (msgSwitch2.exchange(0, std::memory_order_relaxed)){
        std::cerr << "Trigger 2" << std::endl;
    }
}
