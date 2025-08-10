#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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
        std::cerr << "ERROR:SHADER:FILE_NOT_SUCCESSFULLY_READ\n" 
        << "Path: " << shaderPath << "\n"
        << "Error: " <<  e.what()<< std::endl;
        return "";
    }

    return shaderCode;
}

int shader_program(){ 
    // Read shader sources
    std::string vertexCode = loadShaderSource("video-generator-local/shaders/basic.vert");
    std::string fragmentCode = loadShaderSource("video-generator-local/shaders/basic.frag");
    
    if (vertexCode.empty() || fragmentCode.empty()) {
        return -1;
    }
    
    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource= fragmentCode.c_str();

    // Compile vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // Check vertex shader compilation
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }
    std::cerr << "Vertex shader compiled successfully!" << std::endl;

    // Compile fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // Check fragment shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }
    std::cerr << "Fragment shader compiled successfully!" << std::endl;

    // Link shaders into a program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check program linking
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return -1;
    }
    std::cerr << "Shader program linked successfully!" << std::endl;

    // Clean up shaders (no longer needed after linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return 0;
}
