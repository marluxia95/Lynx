cmake_minimum_required(VERSION 3.2.0)
project(Lynx VERSION 0.0.3)

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-lglfw -lGL -lGLEW -lX11 -lpthread -lXrandr -ldl -lassimp -Wwrite-strings -Wconversion-null")


file(COPY res DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(lib)
add_subdirectory(examples/game)
add_subdirectory(LynxEngine)
add_subdirectory(LynxEditor)