#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>

#include <lo/lo.h>
#include <lo/lo_cpp.h>

bool running = true;
void handle_sigint(int){
    running = false;
}

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

    // crear handler function que imprime mensaje recibido
    oscServer.add_method("/lowPerc/trigger", "i",
        [](lo_arg** argv, int){
            std::cerr << argv[0]->i << std::endl;
        }
    );

    // iniciar el servidor
    oscServer.start();

    // event loop, la handler function aparentemente no se tiene que llamar
    while(running){
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // los recursos del servidor se liberan automaticamente
    return 0;
}
