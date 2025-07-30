#include <csignal>              // le permite al programa reaccionar a ctrl + c
#include "program_exit.h"

bool running = true;

// definicion de funcion para detectar señales de la terminal
void handle_sigint(int){
    running = false;
}

// La funcion signal le dice a la funcion handle_sigint que se tiene que ejecutar
// cuando entre la señal SIGINT (signal interrupt ctrl + c)
void program_exit(){
    std::signal(SIGINT, handle_sigint);
}
