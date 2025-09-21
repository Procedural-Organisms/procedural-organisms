#include <glad/glad.h>          // funciones OpenGL
#include <GLFW/glfw3.h>         // windowed context
#include <iostream>             // inputs y outputs de la consola (mensajes de error)
#include <thread>               // le permite al programa dormir o pausar
#include <chrono>               // utilidades de tiempo
#include <cmath>                // funciones matematicas como sin()

#include "../osc_in_loop/osc_in_loop.h"

#include "render_loop.h"

void render_loop(){
    // Render loop que termina cuando se cierra la ventana o se presiona ESC
    while (!glfwWindowShouldClose(window)) {
        // Procesar input
        processInput(window);

        /* Calcular tiempo desde que se inicio el programa */
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - startTime;
        float timeValue = elapsed.count();

        /* Generar funciones seno */
        float normalizedSin1 = (std::sin(2 * M_PI * 0.083f * timeValue) / 2.0f) + 0.5f;
        float normalizedSin2 = (std::sin(2 * M_PI * 0.079f * timeValue) / 2.0f) + 0.5f;
        int sin1Locations = glGetUniformLocation(shaderProgram, "sinGenerator1");
        int sin2Locations = glGetUniformLocation(shaderProgram, "sinGenerator2");
        // TODO cambiar nombre de variables rampGenerator
        int ramp1Locations = glGetUniformLocation(shaderProgram, "rampGenerator1");
        int ramp2Locations = glGetUniformLocation(shaderProgram, "rampGenerator2");

        // ===  TEST  === 
        std::atomic<float> testLeft = osc_in_loop(0);
        float testLeftLocations = glGetUniformLocation(shaderProgram, "testLeft");

        std::atomic<float> testRight = osc_in_loop(1);
        float testRightLocations = glGetUniformLocation(shaderProgram, "testRight");
        if(testRight > 0){
            std::cerr << testRight << std::endl;
        }
        // == == == == == 

        glUseProgram(shaderProgram);
        // ===  TEST  === 
        glUniform1f(testLeftLocations, testLeft);
        glUniform1f(testRightLocations, testRight);
        // == == == == == 
        glUniform1f(sin1Locations, normalizedSin1);
        glUniform1f(sin2Locations, normalizedSin2);
        glUniform1f(ramp1Locations, attRelGenerator1);
        glUniform1f(ramp2Locations, attRelGenerator2);

        // TODO  buscar como llamar a la funcion osc_in_loop con argumentos para obtener distintos valores
        // u otra forma de pasar valores a funcion render_loop

        /* Limpiar color buffer */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        /* Dibujar triangulo */
        glBindVertexArray(VAO); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers y poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // Control de framerate
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
}
