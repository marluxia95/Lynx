cmake_minimum_required(VERSION 3.2.0)
project(Lynx VERSION 0.0.3)

set (CMAKE_CXX_STANDARD 20 )

if(UNIX)
set(CMAKE_CXX_FLAGS "-lglfw -lGL -lGLEW -lX11 -lpthread -lXrandr -ldl -lassimp")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

link_directories( ${CMAKE_BINARY_DIR} )

file(COPY res DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(lib)
add_subdirectory(examples/game)
add_subdirectory(LynxEngine)
add_subdirectory(EngineModules)

# No windows support
if(UNIX)
add_subdirectory(LynxEditor)
endif()