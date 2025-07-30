#ifndef PROGRAM_EXIT_H
#define PROGRAM_EXIT_H

// Variable que le dice al programa si esta corriendo
// la vamos a usar para salir del render loop y finalizar el programa correctamente
extern bool running;

// Declaracion de funcion que detecta el numero de señal que entra por la terminal
// para detectar ctrl + c y finalizar programa correctamente
void handle_sigint(int);

// Declaracion de funcion para cerrar programa cuando se detecte ctrl + c
void program_exit();

#endif
