cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 14)
project(Game VERSION 0.1)

set(CMAKE_CXX_FLAGS "-lglfw -lGL -lGLEW -lX11 -lpthread -lXrandr -ldl -lassimp -Wwrite-strings")

file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/game.cpp
)

add_executable( 
    Game ${SOURCES}
)


target_include_directories(Game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/LynxEngine/src)
target_include_directories(Game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui)
target_include_directories(Game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

include(
    ${CMAKE_CURRENT_SOURCE_DIR}/LynxEngine/cmake/findGLEW.cmake
)

find_package(ASSIMP REQUIRED)
if(ASSIMP_FOUND)
    include_directories(${ASSIMP_INCLUDE_DIR})
endif() 

include_directories( 
    Game

    ${OPENGL_INCLUDE_DIRS}  
    ${GLEW_INCLUDE}
    ${GLFW_INCLUDE}    
    ${ASSIMP_INCLUDE}
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ 
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/glm 
)

target_link_libraries(
    Game

    ${CMAKE_CURRENT_SOURCE_DIR}/LynxEngine/build/libSimpleGameEngine.a
    glfw
    assimp
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${ASSIMP_LIBRARY}
)