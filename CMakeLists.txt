cmake_minimum_required(VERSION 3.2.0)
project(Lynx VERSION 0.0.3)

set(CMAKE_CXX_STANDARD 20 )

set(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE_DIR ${BASE_DIR}/src)
set(DEPS_INSTALL_DIR ${CMAKE_BINARY_DIR})
set(DEPS_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/deps)

if(UNIX)
set(CMAKE_CXX_FLAGS "-lglfw -lGL -lGLEW -lX11 -lpthread -lXrandr -ldl -lassimp")
endif()

cmake_policy(SET CMP0111 NEW)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

link_directories( ${CMAKE_BINARY_DIR} )

file(COPY res DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(deps)
add_subdirectory(src)