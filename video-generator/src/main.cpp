#include <glad/glad.h>          // funciones OpenGL
#include <GL/osmesa.h>          // software offscreen rendering
#include <iostream>             // inputs y outputs de la consola (mensajes de error)
#include <vector>               // buffer de pixeles para offscreen rendering
#include <fstream>              // lectura de archivos
#include <sstream>              // conversionn de contenido de archivos a string
#include <string>               // manejo de strings
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <thread>               // le permite al programa dormir o pausar
#include <chrono>               // utilidades de tiempo
#include <cmath>                // funciones matematicas como sin()

#include "program_exit/program_exit.h"  // funcion para salir del render loop y finalizar programa


// ==================== Declaracion de funciones ====================

// Declacion de funcion que lee un archivo de shader en la direccion que se introduce
// y regresa el contenido de este archivo en un dato de tipo string
std::string loadShaderSource(const char* shaderPath);


// ==================== Declaracion de variables ====================

// Dimensiones de imagen
const int width = 400;
const int height = 300;

/* Tiempo al iniciar programa
    std::chrono::steady_clock es la manera de acceder al clock de la computadora
    y con el metodo .now le decimos al programa que extraiga el valor actual y lo
    guarden en la variable startTime y seleccione automaticamente el tipo de valor
    al que lo tenga que guardar */
auto startTime = std::chrono::steady_clock::now();


// ==================== Definicion de funciones ====================

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


// ==================== Funcion main() ====================

int main(){

    // Salir del render loop y finalizar programa con ctrl + c
    program_exit();

    // ==================== Inicializacion OpenGL ====================

    /* Creacion de buffer de pixeles en memoria RAM:
        creamos un vector donde cada elemento tiene el tamaño de 1 byte o 4 dependiendo
        de OSMesaMakeCurrent (por eso hacemos los elementos de tipo unsigned char o float)
        a ese vector lo llamamos buffer y los hacemos del tamaño de la 
        anchura * altura * 4. el 4 viene de R*G*B*A donde cada una tiene un valor de 1 o 4 
        bytes dependiendo de las opciones de OSMesaMakeCurrent.
        en otras palabras creamos un buffer con el espacio necesario para guardar 
        nuestra imagen generada */
    std::vector<unsigned char> buffer(width * height * 4);


    /* Creacion de contexto OpenGL con OSMesa:
        creamos una variable de tipo OSMesaContext y la llamamos context,
        luego le pasamos la informacion del contexto con la funcion 
        OSMesaCreateContextExt donde "Ext" significa extended. Lo que significa
        que la funcion requiere mas parametros y da mas flexibilidad.
        Sus parametros son GLemum format, GLint depthBits, GLint stencilBits,
        GLint accumBits & OSMesaContext sharelist y sus valores son mas o menos standard */
    OSMesaContext context = OSMesaCreateContextExt(OSMESA_RGBA, 24, 0, 0, NULL);
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
   


   // ==================== Compilacion y linkeo de shaders ====================

   // Lectura de shader sources y convercion a string
   // TODO: Buscar como usar paths relativos
   std::string vertexCode = loadShaderSource("video-generator/shaders/basic.vert");
   std::string fragmentCode = loadShaderSource("video-generator/shaders/basic.frag");
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
    unsigned int shaderProgram = glCreateProgram();
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


    // ==================== Configuracion VBO & VAO ====================

    /* definicion de vertices: 
     guardamos en un array las coordenadas 3D de cada punto de cada triangulo */
     float vertices[] = {
        -0.9f, -0.9f, 0.0f,
        0.9f, -0.9f, 0.0f,
        0.0f, 0.9f, 0.0f
     };


     /* Inicializacion de vertex buffer object y vertex array object:
        el vertex buffer guarda todos los puntos de los vertices y los libera de una
        sola vez al momento de renderizar. Durante hardware rendering transferiria los 
        datos de un array guardado en la RAM hacia la VRAM, y durante software rendering
        es mas acerca de la comunicacion entre el programa y OpenGL y como utilizar
        el espacio en la RAM alocado para el VBO
        El vertex array object (VAO) le dice a la computadora como interpretar los datos 
        del VBO */
     unsigned int VBO;              // variable con ID de VBO
     unsigned int VAO;              // variable con ID de VAO
     glGenBuffers(1, &VBO);         // creacion de VBO y asignacion de ID a variable
     glGenVertexArrays(1, &VAO);    // creacion de VAO y asignacion de ID a variable


     /* Copiar data de vertices array object a VBO:
        con la funcion glBindBuffer le decimos a OpenGl con que tipo de buffer queremos
        trabajar y lo enlazamos a una ID contenida en una variable.
        luego con la funcion glBufferData, le cargamos los datos al buffer, diciendole con
        que buffer queremos trabajar, el espacio que queremos alocar para el, la informacion
        que le queremos cargar, y como queremos usar esta informacion (ej. manera estatica
        o dinamica) */
     glBindBuffer(GL_ARRAY_BUFFER, VBO);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


     /* Copiar data a VAO para decirle a OpenGL como interpretar el VBO:
        con la funcion glBindVertexArray les decimos a OpenGL que objeto queremos cargar
        como VAO.
        la funcion glVertexAttribPointer carga las instrucciones al VAO de como OpenGL
        tiene que interpretar el VBO. 
        - el primer parametro es el index que le dice a OpenGL con que shader comunicarse 
        atravez del VAO (en este caso el valore es 0 porque en nuestro archivo .vert
        escribimos "layout(location = 0)" que se puede comparar con un inlet).
        - el segundo parametro (size) representa el numero de componentes (3=x,y,z).
        - el tercer parametro es el tipo de data de cada componente.
        - el cuarto parametro (normalizacion) junto con el tercero le dice OpenGL si
        tiene que escalar los valores (a -1,1) y como lo tiene que hacer como configuramos
        OpenGL para que espere valores float (-1,1) y estos valores le estamos dando
        entonces desactivamos la normalizacion
        - el quinto parametro (stride) le dice a OpenGL cada cuantos espacios empieza
        un nuevo vertice en el buffer, en este caso 3 bytes, 1 byte para cada componente
        de x*y*z "tightly packed" o sea sin espacio vacios en medio de cada vertice.
        - el ultimo parametro le dice a OpenGL en que poscicion del array comenzar a
        extraer informacion, util cuando el array contiene distintos tipos de informacion
        como color y poscion en espacio y no empieza con la informacion que queremos
        extraer. 
        por ultimo activamos el vertex array con la funcion glEnableVertexAttribArray
        dandole como parametro el index del vertex array que queremos activar. */
     glBindVertexArray(VAO);
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
     glEnableVertexAttribArray(0);


     /* Desvincular VBO y VAO:
        despues de haber vinculado el *V*ertex *B*uffer *O*ject y el
        *V*ertices *A*rray *O*bject y haber transmitido la data a donde tenia que ser
        transmitida, la data queda guardada en ese lugar y podemos desvincul el VBO y
        el VAO lo cual es buena practica para evitar modificaciones accidentales */
     glBindVertexArray(0);
     glBindBuffer(GL_ARRAY_BUFFER, 0);


     // cargar el shader program a OpenGL
     glUseProgram(shaderProgram);


     // ==================== Render loop ====================

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


        /* Generar funcion seno:
            generamos una funcion senoseudal usando el tiempo actual como variable y guardamos
            sus valores en la variable sinNormalizada. lugego con la funcion glGetUniformLocations
            localizamos todas las menciones de la uniforme sinGenerator en en shaderProgram y las
            guardamos en la variable sinesLocations.
            luego que le decimos a OpenGL que utilice ese programa shaderProgram y que vicule 
            nuestra variable en el source sinNormalizada con las localizaciones en el shaderProgram
            donde se menciona sinGenerator */
        float sinNormalizada = (std::sin(timeValue) / 2.0f) + 0.5f;
        int sinesLocations = glGetUniformLocation(shaderProgram,"sinGenerator");
        glUseProgram(shaderProgram);
        glUniform1f(sinesLocations, sinNormalizada);


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
        glDrawArrays(GL_TRIANGLES, 0, 3);
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
        

        // esta funcion le dice al programa que pause por cierta cantidad de milisegundos
        // necesaria para detectar señales de la terminal como ctrl + c para terminar el programa
        // y usada tambien temporalmente para limitar framerate
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
     }

     
    // ==================== Finalizacion del programa ====================

    // Limpieza de recursos y finalizacion de programa
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    OSMesaDestroyContext(context);
    std::cerr << "\nExited render loop and cleared up resources successfully!" << std::endl;
    return 0;
}
