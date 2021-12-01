cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 17)
project(LynxPhysics VERSION 0.1)

file(GLOB_RECURSE SOURCES
    lynxPhysics.cpp
    physicsWorld.cpp
    physicsSystem.cpp
)

add_library( 
    LynxPhysics SHARED ${SOURCES}
)

set(ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/../../LynxEngine/src)
set(LIBS ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

target_include_directories(LynxPhysics PRIVATE $(ENGINE))
target_include_directories(LynxPhysics PRIVATE ${LIBS}/imgui)
target_include_directories(LynxPhysics PRIVATE ${LIBS})

include_directories( 
    LynxPhysics

    ${CMAKE_BINARY_DIR}/include
    ${ENGINE}
    ${LIBS}
    ${LIBS}/imgui
    ${LIBS}/backends
    ${LIBS}/glm 
)