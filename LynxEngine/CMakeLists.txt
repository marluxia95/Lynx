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

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIC -Wwrite-strings" )

link_directories( ${DEPS_INSTALL_DIR} ${DEPS_INSTALL_DIR}/bin ${DEPS_INSTALL_DIR}/lib )

add_library( 
    LynxEngine SHARED ${SOURCES}
)

target_include_directories(LynxEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(LynxEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../lib/imgui)
target_include_directories(LynxEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

find_package(OpenGL REQUIRED)

if(UNIX)
    find_package(GLEW REQUIRED)
    if (GLEW_FOUND)
        include_directories(${GLEW_INCLUDE_DIRS})
        link_libraries(${GLEW_LIBRARIES})
    endif()
elseif(WIN32)
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
    list(APPEND CMAKE_LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
    find_library(GLEW_LIBRARY NAMES GLEW glew32 glew glew32s PATH_SUFFIXES lib64)
    find_package(GLEW REQUIRED)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

include_directories( 
    LynxEngine

    ${CMAKE_BINARY_DIR}/include
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

    lua
    glfw
    assimp
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${ASSIMP_LIBRARY}
)

add_dependencies(LynxEngine glfw_ext assimp_ext)