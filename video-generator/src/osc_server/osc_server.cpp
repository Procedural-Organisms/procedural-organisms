#include <iostream>
#include <atomic>

#include <lo/lo.h>
#include <lo/lo_cpp.h>

#include "osc_server.h"

int osc_server(){

    // crear osc server para reibir mensajes
    // hacerlo de tipo static para que continue cuando acabe la funcion
    static lo::ServerThread oscServer(11303);
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
    

    // ===  TEST  ===
    oscServer.add_method(
        "/test", "f",
        [](lo_arg** argv, int){
            test.store(argv[0]->f, std::memory_order_relaxed);
        }
    );
    // == == == == ==


    // crear handler functions
    // TODO quitar trigger de direccion
    oscServer.add_method("/rightFlash", "iff",
        [](lo_arg** argv, int){
            attack1.store(argv[1]->f, std::memory_order_relaxed);
            release1.store(argv[2]->f, std::memory_order_relaxed);
            if(argv[0]->i == 1){
                msgSwitch1.store(1, std::memory_order_relaxed);
            }
        
        }
    );

    // TODO quitar trigger de direccion
    oscServer.add_method("/leftFlash", "iff",
        [](lo_arg** argv, int){
            attack2.store(argv[1]->f, std::memory_order_relaxed);
            release2.store(argv[2]->f, std::memory_order_relaxed);
            if(argv[0]->i == 1){
                msgSwitch2.store(1, std::memory_order_relaxed);
            }
        }
    );


    // iniciar el servidor
    oscServer.start();


    // los recursos del servidor se liberan automaticamente
    return 0;
}
