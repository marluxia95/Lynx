cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 17)
project(TestModule VERSION 0.1)

file(GLOB_RECURSE SOURCES
    testModule.cpp
)

add_library( 
    TestModule SHARED ${SOURCES}
)

set(ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/../../LynxEngine/src)
set(LIBS ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

target_include_directories(TestModule PRIVATE $(ENGINE))
target_include_directories(TestModule PRIVATE ${LIBS}/imgui)
target_include_directories(TestModule PRIVATE ${LIBS})

include_directories( 
    TestModule

    ${CMAKE_BINARY_DIR}/include
    ${ENGINE}
    ${LIBS}
    ${LIBS}/imgui
    ${LIBS}/backends
    ${LIBS}/glm 
)