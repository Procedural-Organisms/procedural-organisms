#include <iostream>
#include <atomic>

#include <lo/lo.h>
#include <lo/lo_cpp.h>

#include "osc_server.h"

int osc_server(){

    // crear osc server para reibir mensajes
    // hacerlo de tipo static para que continue cuando acabe la funcion
    static lo::ServerThread oscServer(13939);
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
    oscServer.add_method(
        "/leftColor", "fff",
        [](lo_arg** argv, int){
            envelopeLeft.store(argv[0]->f, std::memory_order_relaxed);
            param1Left.store(argv[1]->f, std::memory_order_relaxed);
            param2Left.store(argv[2]->f, std::memory_order_relaxed);
        }
    );

    oscServer.add_method(
        "/rightColor", "fff",
        [](lo_arg** argv, int){
            envelopeRight.store(argv[0]->f, std::memory_order_relaxed);
            param1Right.store(argv[1]->f, std::memory_order_relaxed);
            param2Right.store(argv[2]->f, std::memory_order_relaxed);
        }
    );


    // iniciar el servidor
    oscServer.start();


    // los recursos del servidor se liberan automaticamente
    return 0;
}
