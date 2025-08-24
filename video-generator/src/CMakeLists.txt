# informacion de proyecto y versiones
cmake_minimum_required(VERSION 3.10)
project(video_generator)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# le digo a cmake que agrege todos los archivos cpp en los subfolderes al compilado
file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

set(GLAD_C "${CMAKE_CURRENT_SOURCE_DIR}/../libs/glad.c")

# le digo a cmake que cree un ejecutable con mis archivos
add_executable(video_generator
    ${SRC_FILES}
    ${GLAD_C}
)

target_include_directories(video_generator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../includes
)

if(APPLE)
    set(OSMESA_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../libs/libOSMesa.dylib)
    set(LIBLO_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../libs/liblo.dylib)
    set(LIBGLAPI_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../libs/libglapi.dylib)
elseif(UNIX)
    set(OSMESA_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../libs/libOSMesa.so)
    set(LIBLO_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../libs/liblo.so)
    set(LIBGLAPI_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../libs/libglapi.so)
endif()

target_link_libraries(video_generator PRIVATE
    ${OSMESA_LIB}
    ${LIBLO_LIB}
    ${LIBGLAPI_LIB}
)

set_target_properties(video_generator PROPERTIES
    BUILD_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/../libs"
    INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/../libs"
)

# creo una variable con la localizacion del source para pasarsela
# al programa y que logre localizar los shaders
target_compile_definitions(video_generator PRIVATE
    SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)
