#include <glad/glad.h>          // funciones OpenGL
#include <GL/osmesa.h>          // software offscreen rendering
#include <iostream>             // inputs y outputs de la consola (mensajes de error)
#include <vector>               // buffer de pixeles para offscreen rendering
#include <fstream>              // lectura de archivos
#include <sstream>              // conversionn de contenido de archivos a string
#include <string>               // manejo de strings
#include <thread>               // le permite al programa dormir o pausar
#include <chrono>               // utilidades de tiempo
#include <cmath>                // funciones matematicas como sin()

#include "program_exit/program_exit.h"      // funcion para salir del render loop y finalizar programa
#include "shader_program/shader_program.h"  // funcion para compilar shaders y crear shader program
#include "buffer_configuration/buffer_configuration.h"    // configuracion de VBO y VAO
#include "gl_context/gl_context.h"          // creacion de contexto OSMesa
#include "render_loop/render_loop.h"        // render loop OpenGL

// ==================== Declaracion de variables ====================

// Variable que determina si el programa esta corriendo
bool running = true;

/* Tiempo al iniciar programa
    std::chrono::steady_clock es la manera de acceder al clock de la computadora
    y con el metodo .now le decimos al programa que extraiga el valor actual y lo
    guarden en la variable startTime y seleccione automaticamente el tipo de valor
    al que lo tenga que guardar */
std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

// Dimensiones de imagen
const int width = 400;
const int height = 300;

/* Creacion de buffer de pixeles en memoria RAM:
    creamos un vector donde cada elemento tiene el tamaño de 1 byte o 4 dependiendo
    de OSMesaMakeCurrent (por eso hacemos los elementos de tipo unsigned char o float)
    a ese vector lo llamamos buffer y los hacemos del tamaño de la 
    anchura * altura * 4. el 4 viene de R*G*B*A donde cada una tiene un valor de 1 o 4 
    bytes dependiendo de las opciones de OSMesaMakeCurrent.
    en otras palabras creamos un buffer con el espacio necesario para guardar 
    nuestra imagen generada */
std::vector<unsigned char> buffer(width * height * 4);

// Contexto de OpenGL
OSMesaContext context = nullptr;

// Variable donde se guardan los shader linkeados
unsigned int shaderProgram = 0;

// Variable con ID de VBO
unsigned int VBO;   
 // Variable con ID de VAO           
unsigned int VAO;   

// ==================== Funcion main() ====================

int main(){

    // Salir del render loop y finalizar programa con ctrl + c
    program_exit();

    // ==================== Inicializacion OpenGL ====================

    gl_context();


    // ==================== Compilacion y linkeo de shaders ====================
    
    shader_program();


    // ==================== Configuracion VBO & VAO ====================

    buffer_configuration();

    
    // ==================== Render loop ====================

    render_loop();

     
    // ==================== Finalizacion del programa ====================

    // Limpieza de recursos y finalizacion de programa
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    OSMesaDestroyContext(context);
    std::cerr << "\nExited render loop and cleared up resources successfully!" << std::endl;
    return 0;
}
