#include <glad/glad.h>          // funciones OpenGL
#include <GL/osmesa.h>          // software offscreen rendering
#include <iostream>             // inputs y outputs de la consola (mensajes de error)
#include <fstream>              // lectura de archivos
#include <sstream>              // conversionn de contenido de archivos a string
#include <string>               // manejo de strings

#include "shader_program.h"

// Definicion de funcion para convertir contenido de archivo de shaders a string
std::string loadShaderSource(const char* shaderPath){
    // Declaracion de objetos
    std::string shaderCode;
    std::ifstream shaderFile;

    // Deteccion de errores
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try{
        shaderFile.open(shaderPath);        // Abrir archivo en objeto shaderFile
        std::stringstream shaderStream;     // Declaracion de objeto
        shaderStream << shaderFile.rdbuf(); // Buffer intermediario entre archivo y string
        shaderFile.close();                 // Cerrar archivo
        shaderCode = shaderStream.str();    // convertir a string
    }
    catch(std::ifstream::failure e){
    // Generacion de mensaje de error
    // OJO: Se tiene que usar cerr envez de cout porque cout corrompe el stream de datos por pipe
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" 
        << "Path: " << shaderPath << "\n"
        << "Error: " <<  e.what()<< std::endl;
        return "";
    }

    return shaderCode;
}

int shader_program(){ 
 // ==================== Compilacion y linkeo de shaders ====================
   #ifndef SRC_DIR
   #define SRC_DIR
   #endif

   // Lectura de shader sources y convercion a string
   // TODO: Buscar como usar paths relativos
   std::string vertexCode = loadShaderSource((std::string(SRC_DIR) + "/../shaders/basic.vert").c_str());
   std::string fragmentCode = loadShaderSource((std::string(SRC_DIR) + "/../shaders/basic.frag").c_str());
   const char* vertexShaderSource = vertexCode.c_str();
   const char* fragmentShaderSource= fragmentCode.c_str();


    /* Compilacion de vertex shader:
        se carga vertexShaderSource en vertexShader que luego se compila */
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Deteccion de errores
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        // Mensaje si la compilacion de vertex shader falla
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        // Destruccion de contexto y finalizacion de programa
        OSMesaDestroyContext(context);
        return -1;
    }
    // Mensaje si se logra compilar el vertex shader
    std::cerr << "Vertex shader compiled successfully!" << std::endl;


    /* Compilacion de fragment shader:
        se carga fragmentShaderSource en fragmentShader que luego se compila */
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Deteccion de errores
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        // Mensaje si la compilacion de fragment shader falla
        std::cerr << "ERROR::SHADER::FRAGMENT:COMPILATION_FAILED" << infoLog << std::endl;
        // destruccion de contexto y vertex shader y finalizacion de programa
        glDeleteShader(vertexShader);
        OSMesaDestroyContext(context);
    }
    // mensaje si se logra compilar el fragment shader
    std::cerr << "Fragment shader compiled successfully!" << std::endl;
  

    /* Creacion de shader program y linkeo de shaders:
        creamos algo llamado shader program donde juntamos los shaders individuales*/
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Deteccion de errores
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        // mensaje si el linkeo de shader falla
        std::cerr << "ERROR:SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        OSMesaDestroyContext(context);
        return -1;
    }
    // mensaje si se logran linkear los shaders
    std::cerr << "Shader program linked successfully!" << std::endl;
    // eliminacion de shaders ya linkeados
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    return 0;
}
