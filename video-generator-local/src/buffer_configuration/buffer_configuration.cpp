#include <glad/glad.h>          // funciones OpenGL

#include "buffer_configuration.h"    // configuracion de VBO y VAO

void buffer_configuration(){
    
    // ==================== Configuracion VBO & VAO ====================

    /* definicion de vertices: 
      guardamos en un array las coordenadas 3D de cada punto que queremos guardar */
     float vertices[] = {
         -1, 1, 0,
         -1, -1, 0,
         1, 1, 0,
         1, -1, 0,
     };
     
   
    /* difinicion de indices
      guardamos en un array la direcion de cada punto que queremos dibujar */
     unsigned int indices[] = {
         0, 1, 3,
         0, 2, 3,
     };


     /* Inicializacion de vertex buffer object y vertex array object:
        el vertex buffer guarda todos los puntos de los vertices y los libera de una
        sola vez al momento de renderizar. Durante hardware rendering transferiria los 
        datos de un array guardado en la RAM hacia la VRAM, y durante software rendering
        es mas acerca de la comunicacion entre el programa y OpenGL y como utilizar
        el espacio en la RAM alocado para el VBO
        El vertex array object (VAO) le dice a la computadora como interpretar los datos 
        del VBO
        con el EBO podemos usar direcciones de puntos para dibujar envez de tener que
        escribir cada coordenada */
     glGenBuffers(1, &VBO);         // creacion de VBO y asignacion de ID a variable
     glGenVertexArrays(1, &VAO);    // creacion de VAO y asignacion de ID a variable
     glGenBuffers(1, &EBO);         // creacion de EBO y asignacion de ID a variable


     /* Copiar data de vertices array object a VBO:
        con la funcion glBindBuffer le decimos a OpenGl con que tipo de buffer queremos
        trabajar y lo enlazamos a una ID contenida en una variable.
        luego con la funcion glBufferData, le cargamos los datos al buffer, diciendole con
        que buffer queremos trabajar, el espacio que queremos alocar para el, la informacion
        que le queremos cargar, y como queremos usar esta informacion (ej. manera estatica
        o dinamica) */
     glBindBuffer(GL_ARRAY_BUFFER, VBO);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
     

     // copiar data de indices a EBO
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



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


     /* Desvincular VBO, VAO y EBO:
        despues de haber vinculado el *V*ertex *B*uffer *O*ject y el
        *V*ertices *A*rray *O*bject y haber transmitido la data a donde tenia que ser
        transmitida, la data queda guardada en ese lugar y podemos desvincul el VBO y
        el VAO lo cual es buena practica para evitar modificaciones accidentales */
     glBindVertexArray(0);
     glBindBuffer(GL_ARRAY_BUFFER, 0);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


     // cargar el shader program a OpenGL
     glUseProgram(shaderProgram);
}
