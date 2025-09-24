#include <glad/glad.h>          // funciones OpenGL
#include <GLFW/glfw3.h>         // windowed context
#include <iostream>             // inputs y outputs de la consola (mensajes de error)
#include <thread>               // le permite al programa dormir o pausar
#include <chrono>               // utilidades de tiempo
#include <cmath>                // funciones matematicas como sin()

#include "render_loop.h"

void render_loop(){
    // Render loop que termina cuando se cierra la ventana o se presiona ESC
    while (!glfwWindowShouldClose(window)){
        // Procesar input
        processInput(window);

        /* Calcular tiempo desde que se inicio el programa */
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - startTime;
        float timeValue = elapsed.count();


        // ===  TEST  === 
        /* Guardar mensajes continuos osc en variables atomicas y mandarlas
            a las uniformes dentro de los shaders */
        float envelopeLeftLocations = glGetUniformLocation(shaderProgram, "envelopeLeft");
        float param1LeftLocations = glGetUniformLocation(shaderProgram, "param1Left");
        float param2LeftLocations = glGetUniformLocation(shaderProgram, "param2Left");

        float envelopeRightLocations = glGetUniformLocation(shaderProgram, "envelopeRight");
        float param1RightLocations = glGetUniformLocation(shaderProgram, "param1Right");
        float param2RightLocations = glGetUniformLocation(shaderProgram, "param2Right");

        glUseProgram(shaderProgram);

        glUniform1f(envelopeLeftLocations, envelopeLeft.load(std::memory_order_relaxed));
        glUniform1f(param1LeftLocations, param1Left.load(std::memory_order_relaxed));
        glUniform1f(param2LeftLocations, param2Left.load(std::memory_order_relaxed));

        glUniform1f(envelopeRightLocations, envelopeRight.load(std::memory_order_relaxed));
        glUniform1f(param1RightLocations, param1Right.load(std::memory_order_relaxed));
        glUniform1f(param2RightLocations, param2Right.load(std::memory_order_relaxed));
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
