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

        /* Guardar mensajes continuos osc en variables atomicas y mandarlas
            a las uniformes dentro de los shaders */
        // ===  TEST  === 
        testLeft.store(osc_in_loop(0), std::memory_order_relaxed);
        float testLeftLocations = glGetUniformLocation(shaderProgram, "envelopeLeft");

        testRight.store(osc_in_loop(1), std::memory_order_relaxed);
        float testRightLocations = glGetUniformLocation(shaderProgram, "envelopeRight");
        if(testRight > 0){
            std::cerr << testRight << std::endl;
        }

        glUseProgram(shaderProgram);
        
        // ===  TEST  === 
        glUniform1f(testLeftLocations, testLeft);
        glUniform1f(testRightLocations, testRight);
        // == == == == == 

        /* Limpiar color buffer */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        /* Dibujar vertices */
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
