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

target_include_directories(LynxPhysics PRIVATE ${SOURCE_DIR}/LynxEngine)
target_include_directories(LynxPhysics PRIVATE ${LIBS}/imgui)
target_include_directories(LynxPhysics PRIVATE ${LIBS})

target_link_directories(LynxPhysics PRIVATE ${DEPS_INSTALL_DIR}/lib )

include_directories( 
    LynxPhysics

    ${CMAKE_BINARY_DIR}/include
    ${DEPS_INCLUDE_DIR}
    ${SOURCE_DIR}/LynxEngine
)

target_link_libraries(
	LynxPhysics

	LynxEngine
)

add_dependencies(LynxPhysics LynxEngine)