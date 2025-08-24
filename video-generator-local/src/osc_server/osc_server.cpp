#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <atomic>

#include <lo/lo.h>
#include <lo/lo_cpp.h>

bool running = true;
void handle_sigint(int){
    running = false;
}

std::atomic<int> msgSwitch1 = 0;
std::atomic<int> msgSwitch2 = 0;

int main(){
    // programm exit
    std::signal(SIGINT, handle_sigint);


    // crear osc server para reibir mensajes
    lo::ServerThread oscServer(11302);
    if(!oscServer.is_valid()){
        std::cerr << "ERROR::OSC::SERVER_NOT_SUCCESSFULLY_STARTED" << std::endl;
        return -1;
    }


    // crear mensajes al iniciar y cerrar servidor
    oscServer.set_callbacks(
        [](){
            std::cerr << "OSC server started successfully!" << std::endl;},
        [](){
            std::cerr << "OSC server closed successfully!" << std::endl;}
    );
    // imprimir URL del servidor UDP
    std::cerr << "OSC server URL: " << oscServer.url() << std::endl;


    // crear handler functions
    oscServer.add_method("/lowPerc/trigger", "i",
        [](lo_arg** argv, int){
            if(argv[0]->i == 1){
                msgSwitch1.store(1, std::memory_order_relaxed);
            }
        }
    );

    oscServer.add_method("/hiPerc/trigger", "i",
        [](lo_arg** argv, int){
            if(argv[0]->i == 1){
                msgSwitch2.store(1, std::memory_order_relaxed);
            }
        }
    );


    // iniciar el servidor
    oscServer.start();


    // event loop, la handler function aparentemente no se tiene que llamar
    while(running){
        // la funcion exchange se activa cuando msgSwitch cambia a 1
        // activando la funcion dentro de la condicional 
        // y directamente cambiando el valor de msgSwitch de vuelta a 0
        if(msgSwitch1.exchange(0, std::memory_order_relaxed)){
            std::cerr << "Trigger 1" << std::endl;
        }

        if (msgSwitch2.exchange(0, std::memory_order_relaxed)){
            std::cerr << "Trigger 2" << std::endl;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    
    // los recursos del servidor se liberan automaticamente
    return 0;
}
