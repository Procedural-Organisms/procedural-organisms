#include <iostream>
#include <lo/lo.h>
#include <lo/lo_cpp.h>

int main(){
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
            std::cerr << "OSC server closed successfully!";}
    );
    std::cerr << "URL: " << oscServer.url() << std::endl;

    return 0;
}
