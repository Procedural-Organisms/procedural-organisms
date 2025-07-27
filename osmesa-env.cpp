#include <glad/glad.h>          // funciones OpenGL
#include <GL/osmesa.h>          // software offscreen rendering
#include <iostream>             // inputs y outputs de la consola (mensajes de error)
#include <vector>               // buffer de pixeles para offscreen rendering
#include <fstream>              // lectura de archivos
#include <sstream>              // conversionn de contenido de archivos a string
#include <string>               // manejo de strings
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>    // generacion de archivos PNG

// Declacion de funcion que lee un archivo de shader en la direccion que se introduce
// y regresa el contenido de este archivo en un dato de tipo string
std::string loadShaderSource(const char* shaderPath);

// Dimensiones de imagen
const int width = 800;
const int height = 600;

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
        std::cout << "ERROR:SHADER:FILE_NOT_SUCCESSFULLY_READ\n"
        << "Path: " << shaderPath << "\n"
        << "Error: " <<  e.what()<< std::endl;
        return "";
    }

    return shaderCode;
}

int main(){
    

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
        std::cout << "Failed to create OSMesa context" << std::endl;
        // finalizacion de programa
        return -1;
    }
        // Mensaje si el contexto se logra crear
    std::cout << "OSMesa context created successfully!" << std::endl;


    /* Hacer que el contexto sea actual:
        con la funcion OSMesaMakeCurrent le decimos a OpenGL que contexto utilizar.
        podemos programar multiples contextos e intercambiar entre ellos.
        sus parametros son el contexto que queremos utilizar, la localizacion del
        buffer en la RAM (con .data()), el tipo de data que queeremos usar para cada
        componente de color (GL_UNSIGNED_BYTE = 1 byte o GL_FLOAT = 4 bytes), la 
        anchura de la imagen y la altura de la imagen */
    if (!OSMesaMakeCurrent(context, buffer.data(), GL_UNSIGNED_BYTE, width, height)){
        // Mensaje de error si hacer que el contexto sea actual falla, 
        std::cout << "Failed to make context current" << std::endl;
        // destruccion de contexto y finalizacion de programa
        OSMesaDestroyContext(context);
        return -1;
    }
        // Mensaje si se logra hacer que el contexto sea actual
    std::cout << "Context made current successfully!" << std::endl;


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
        std::cout << "Failed to initialize GLAD" << std::endl;
        // destruccion de contexto y finalizacion de programa
        OSMesaDestroyContext(context);
        return -1;
   }
        // Mensaje si se logra inicializar GLAD
   std::cout << "GLAD initialized successfully!" << std::endl;


   // Lectura de shader sources y convercion a string
   // TODO: Buscar como usar paths relativos
   std::string vertexCode = loadShaderSource("/Users/elektronischesstudio/Documents/01 proyectos/practica_opengl/00_LearnOpenGL_propio/basic.vert");
   std::string fragmentCode = loadShaderSource("/Users/elektronischesstudio/Documents/01 proyectos/practica_opengl/00_LearnOpenGL_propio/basic.frag");
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
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        // Destruccion de contexto y finalizacion de programa
        OSMesaDestroyContext(context);
        return -1;
    }
    // Mensaje si se logra compilar el vertex shader
    std::cout << "Vertex shader compiled successfully!" << std::endl;


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
        std::cout << "ERROR::SHADER::FRAGMENT:COMPILATION_FAILED" << infoLog << std::endl;
        // destruccion de contexto y vertex shader y finalizacion de programa
        glDeleteShader(vertexShader);
        OSMesaDestroyContext(context);
    }
    // mensaje si se logra compilar el fragment shader
    std::cout << "Fragment shader compiled successfully!" << std::endl;
  

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
        std::cout << "ERROR:SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        OSMesaDestroyContext(context);
        return -1;
    }
    // mensaje si se logran linkear los shaders
    std::cout << "Shader program linked successfully!" << std::endl;
    // eliminacion de shaders ya linkeados
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    /* definicion de vertices: 
     guardamos en un array las coordenadas 3D de cada punto de cada triangulo */
     float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
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


     /* definir un color con el cual limpiar el color buffer y limpiarlo:
        glClearColor se usa para definir el color con el que se va a limpiar el color
        buffer y glClear se usa para especificar que buffer se quiere limpiar y 
        limpiarlo, en este caso el color buffer. Se pueden limpiar varios buffers con
        una sola funcion separandolos con "|" */
     glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
     glClear(GL_COLOR_BUFFER_BIT);


     // cargar el shader program a OpenGL
     glUseProgram(shaderProgram);


     // Creacion de render loop
     while(true){
        /* Volver a vincular VAO y dibujar una imagen
            volvemos a cargar el vertex array object para decirle a OpenGL como interpretar
            los datos que ya estan cargados en el buffer.
            luego utilizamos la funcion glDrawArrays para dibujar una forma usando los datos
            guardados en el buffer y las instrucciones del VAO
            - el primer parametro el el tipo de primitiva, si va a dibujar puntos, lineas o
            triangulos
            - el segundo parametro es el indice de incicio que le dice al programa con que 
            vertice empezar el dibujo
            - el tercer parametro le dice al programa cuantos vertices dibujar */
        glBindVertexArray(VAO); 
        glDrawArrays(GL_TRIANGLES, 0, 3);
     }

     
    // Limpieza de recursos y finalizacion de programa
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    OSMesaDestroyContext(context);
    return 0;
}
