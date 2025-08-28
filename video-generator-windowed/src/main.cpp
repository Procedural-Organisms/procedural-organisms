#include <glad/glad.h>          // funciones OpenGL
#include <GLFW/glfw3.h>         // windowed rendering context
#include <iostream>             // inputs y outputs de la consola (mensajes de error)
#include <chrono>               // utilidades de tiempo
#include <atomic>               // variables atomicas (multithreading posible)

#include "program_exit/program_exit.h"
#include "shader_program/shader_program.h"  
#include "buffer_configuration/buffer_configuration.h"
#include "gl_context/gl_context.h"
#include "render_loop/render_loop.h"
#include "osc_server/osc_server.h"

// ==================== Declaracion de variables ====================
bool running = true;

// Tiempo al iniciar programa
std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

// Variables para OSC tigger
std::atomic<int> msgSwitch1 = 0;
std::atomic<int> msgSwitch2 = 0;

// Dimensiones de ventana
const int width = 800;
const int height = 600;

// Ventana GLFW
GLFWwindow* window = nullptr;

// Variable donde se guardan los shader linkeados
unsigned int shaderProgram = 0;

// Variables con IDs de buffers
unsigned int VBO;
unsigned int VAO;
unsigned int EBO;

int main(){
    // Inicializar contexto OpenGL con GLFW
    if(gl_context() != 0) {
        return -1;
    }

    // Compilar y linkear shaders
    if(shader_program() != 0) {
        glfwTerminate();
        return -1;
    }

    // Configurar buffers
    buffer_configuration();

    // sevidor OSC
    osc_server();

    // Render loop
    render_loop();


    // Limpieza
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    
    std::cerr << "\nExited successfully!" << std::endl;
    return 0;
}
