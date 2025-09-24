#include <glad/glad.h>          // funciones OpenGL
#include <iostream>             // inputs y outputs de la consola (mensajes de error)
#include <thread>               // le permite al programa dormir o pausar
#include <chrono>               // utilidades de tiempo
#include <cmath>                // funciones matematicas como sin()
#include <fstream>              // creacion de flags
#include <filesystem>           // lectura de flags

#include "render_loop.h"

void render_loop(){

    // variable para avisarle al programa que la flag para iniciar audio no existe
    bool flag_exists = false;

    // esperar a que ffmpeg comience
    while(!std::filesystem::exists("temp/flags/ffmpeg_started.flag")){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Creacion de render loop que termina cuando running = false (ctrl - c)
     while(running){
        /* Calcular tiempo desde que se inicio el programa:
            con cada repeticion de loop guardamos el tiempo actual en la variable now, a esa
            variable le restamos el tiempo en el que iniciamos el programa y usando
            std::chrono::duration<float> le decimos al programa que guarde ese valor en segundos
            float en la variable timeValue */
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - startTime;
        float timeValue = elapsed.count();


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


        /* definir un color con el cual limpiar el color buffer y limpiarlo:
            glClearColor se usa para definir el color con el que se va a limpiar el color
            buffer y glClear se usa para especificar que buffer se quiere limpiar y 
            limpiarlo, en este caso el color buffer. Se pueden limpiar varios buffers con
            una sola funcion separandolos con "|" */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        /* Volver a vincular VAO y dibujar una imagen:
            volvemos a cargar el vertex array object para decirle a OpenGL como interpretar
            los datos que ya estan cargados en el buffer.
            luego utilizamos la funcion glDrawArrays para dibujar una forma usando los datos
            guardados en el buffer y las instrucciones del VAO
            - el primer parametro el el tipo de primitiva, si va a dibujar puntos, lineas o
            triangulos
            - el segundo parametro es el indice de incicio que le dice al programa con que 
            vertice empezar el dibujo
            - el tercer parametro le dice al programa cuantos vertices dibujar
            Por ultima le decimos a OpenGL con glFlush que ejecute los comandos de dibujo */
        glBindVertexArray(VAO); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glFlush(); 


        /* Transmitir datos a FFmpeg:
        con .write le decimos a std::cout que transmita datos binarios en vez de texo
        esta funcion necesita 2 parametros, la direccion de donde se encuentra la data la cual
        extraemos con buffer.data pero convertida a char con reinterpret_cast. 
        y el segundo parametro representa la cantidad de bits que se quieren transmitir.
        4 bytes por pixel que representan R*G*B*A.
        luego std::cout.flush(); transmite inmediatamente la data al output del programa sin esperar a
        que el buffer se llene por completo */
        std::cout.write(reinterpret_cast<char*>(buffer.data()), width * height * 4);
        std::cout.flush();


        // crear flag para iniciar transmicion de audio directamente despues del primer frame
        if(!flag_exists){
            std::ofstream flag("temp/flags/video_started.flag");
            flag.close();
            flag_exists = true;
        }
        
        // esta funcion le dice al programa que pause por cierta cantidad de milisegundos
        // necesaria para detectar señales de la terminal como ctrl + c para terminar el programa
        // y usada tambien temporalmente para limitar framerate
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
     }
}
