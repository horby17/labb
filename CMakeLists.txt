cmake_minimum_required(VERSION 3.28)
project(newlab)

set(CMAKE_CXX_STANDARD 17)

# Establece la ruta de vcpkg como una herramienta de CMake, si no se ha hecho automáticamente.
set(CMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
        CACHE STRING "Vcpkg toolchain file")

# Usamos vcpkg, no es necesario definir manualmente las rutas de CMAKE_MODULE_PATH para glfw o GLAD
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(glad CONFIG REQUIRED)  # Usam${Stb_INCLUDE_DIR}os el modo CONFIG que es más moderno y robusto
find_package(Stb REQUIRED)

add_executable(newlab source_files/main.cpp
        header_files/EBO.h
        header_files/glad.h
        header_files/shaderClass.h
        header_files/VBO.h
        header_files/VAO.h
        source_files/EBO.cpp
        source_files/VAO.cpp
        source_files/VBO.cpp
        source_files/shaderClass.cpp
        source_files/stb_image.h
        source_files/Textures.cpp
        header_files/Texture.h
        source_files/stb_image.cpp
)


target_include_directories(newlab PRIVATE ${Stb_INCLUDE_DIR})
target_link_libraries(newlab PRIVATE glfw GL GLEW::glew glad::glad)