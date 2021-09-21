file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/imgui/imgui.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/imgui/imgui_demo.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/imgui/imgui_draw.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/imgui/imgui_tables.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/imgui/backends/imgui_impl_glfw.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/imgui/backends/imgui_impl_opengl3.cpp
)

add_library( 
    LynxEngine ${SOURCES}
)

target_include_directories(LynxEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(LynxEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../lib/imgui)
target_include_directories(LynxEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

find_package(OpenGL REQUIRED)
#find_package(glfw3 3.3 REQUIRED)

include(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/findGLEW.cmake
)

find_package(ASSIMP REQUIRED)
if(ASSIMP_FOUND)
    include_directories(${ASSIMP_INCLUDE_DIR})
endif() 

include_directories( 
    LynxEngine

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
    LynxEngine

    glfw
    assimp
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${ASSIMP_LIBRARY}
)

add_dependencies(LynxEngine glfw_ext)

install(TARGETS LynxEngine
    LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/bin
    ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/bin
)