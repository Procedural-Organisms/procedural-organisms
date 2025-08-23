# informacion de proyecto y versiones
cmake_minimum_required(VERSION 3.10)
project(local_video_generator)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# le digo a cmake que agrege todos los archivos cpp en los subfolderes al compilado
file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

set(GLAD_C "${CMAKE_CURRENT_SOURCE_DIR}/../libs/glad.c")

# le digo a cmake que cree un ejecutable con mis archivos
add_executable(local_video_generator
    ${SRC_FILES}
    ${GLAD_C}
)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)

target_include_directories(local_video_generator PRIVATE
    ${GLFW3_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../includes
) 

target_link_directories(local_video_generator PRIVATE
    ${GLFW3_LIBRARY_DIRS}
)

target_link_libraries(local_video_generator PRIVATE
    ${GLFW3_LIBRARIES}
    OpenGL::GL
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
)

# creo una variable con la localizacion del source para pasarsela
# al programa y que logre localizar los shaders
target_compile_definitions(local_video_generator PRIVATE
    SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)
