#include <glad/glad.h>          // funciones OpenGL
#include <GL/osmesa.h>          // software offscreen rendering
#include <iostream>             // inputs y outputs de la consola (mensajes de error)
#include <vector>               // buffer de pixeles para offscreen rendering

#include "gl_context.h"

int gl_context(){

    /* Creacion de contexto OpenGL con OSMesa:
        creamos una variable de tipo OSMesaContext y la llamamos context,
        luego le pasamos la informacion del contexto con la funcion 
        OSMesaCreateContextExt donde "Ext" significa extended. Lo que significa
        que la funcion requiere mas parametros y da mas flexibilidad.
        Sus parametros son GLemum format, GLint depthBits, GLint stencilBits,
        GLint accumBits & OSMesaContext sharelist y sus valores son mas o menos standard */
    context = OSMesaCreateContextExt(OSMESA_RGBA, 24, 0, 0, NULL);
    if (!context){
        // Mensaje de error si creacion de contexto falla
        std::cerr << "Failed to create OSMesa context" << std::endl;
        // finalizacion de programa
        return -1;
    }
        // Mensaje si el contexto se logra crear
    std::cerr << "OSMesa context created successfully!" << std::endl;


    /* Hacer que el contexto sea actual:
        con la funcion OSMesaMakeCurrent le decimos a OpenGL que contexto utilizar.
        podemos programar multiples contextos e intercambiar entre ellos.
        sus parametros son el contexto que queremos utilizar, la localizacion del
        buffer en la RAM (con .data()), el tipo de data que queeremos usar para cada
        componente de color (GL_UNSIGNED_BYTE = 1 byte o GL_FLOAT = 4 bytes), la 
        anchura de la imagen y la altura de la imagen */
    if (!OSMesaMakeCurrent(context, buffer.data(), GL_UNSIGNED_BYTE, width, height)){
        // Mensaje de error si hacer que el contexto sea actual falla, 
        std::cerr << "Failed to make context current" << std::endl;
        // destruccion de contexto y finalizacion de programa
        OSMesaDestroyContext(context);
        return -1;
    }
        // Mensaje si se logra hacer que el contexto sea actual
    std::cerr << "Context made current successfully!" << std::endl;


    /* Inicializacion de GLAD:
    GLAD es una libreria que le permite a nuesto codigo interactuar con funciones de 
    Modern OpenGL que dependiendo de la plataforma, estan guardadas en lugares distintos.
    GLAD le dice a nuestro programa donde estan guardadas esas funciones.
    OSMesaGetProcAddress encuentra las direcciones de las fuciones de Modern OpenGL para
    software rendering y las guarda en un valor tipo GLADloadproc tipo GLADloadproc y 
    gladLoadGLLoader,al que le le decimos que espere un valor de tipo GLADloadproc 
    las carga */
   if (!gladLoadGLLoader((GLADloadproc)OSMesaGetProcAddress)){
        // Mensaje si la inicializacion de GLAD falla, 
        std::cerr << "Failed to initialize GLAD" << std::endl;
        // destruccion de contexto y finalizacion de programa
        OSMesaDestroyContext(context);
        return -1;
   }
        // Mensaje si se logra inicializar GLAD
   std::cerr << "GLAD initialized successfully!" << std::endl;

   return 0;
}
