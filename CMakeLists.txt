cmake_minimum_required(VERSION 3.2.0)
project(Lynx VERSION 0.0.3)

set (CMAKE_CXX_STANDARD 17)
if(UNIX)
set(CMAKE_CXX_FLAGS "-lglfw -lGL -lGLEW -lX11 -lpthread -lXrandr -ldl -lassimp -Wwrite-strings -Wreorder -Wconversion-null")
set(GLEW_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(GLEW_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/lib)
endif()

link_directories( ${CMAKE_BINARY_DIR}/lib )
link_directories( ${CMAKE_BINARY_DIR}/bin )

file(COPY res DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(lib)
add_subdirectory(examples/game)
add_subdirectory(LynxEngine)

# No windows support
if(UNIX)
add_subdirectory(LynxEditor)
endif()