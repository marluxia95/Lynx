cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 17)
project(Game VERSION 0.1)

if(UNIX)
set(CMAKE_CXX_FLAGS "-lglfw -lGL -lGLEW -lX11 -lpthread -pthread -lXrandr -ldl -lassimp -Wwrite-strings")
endif()

file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/game.cpp
)

add_executable( 
    Game ${SOURCES}
)

set(ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/../../LynxEngine/src)
set(LIBS ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

target_include_directories(Game PRIVATE $(ENGINE))
target_include_directories(Game PRIVATE ${LIBS}/imgui)
target_include_directories(Game PRIVATE ${LIBS})

find_package(OpenGL REQUIRED)

include_directories( 
    Game

    ${CMAKE_BINARY_DIR}/include
    ${OPENGL_INCLUDE_DIRS}  
    ${GLEW_INCLUDE}
    ${GLFW_INCLUDE}    
    ${ASSIMP_INCLUDE}
    ${ENGINE}
    ${LIBS}
    ${LIBS}/imgui
    ${LIBS}/backends
    ${LIBS}/glm 
)

if(UNIX)
	target_link_libraries(
		Game

		LynxEngine
		glfw
		assimp
		${OPENGL_LIBRARIES}
		${GLEW_LIBRARY}
		${ASSIMP_LIBRARY}
	)
elseif(WIN32)
	target_link_libraries(
		Game

		LynxEngine
		glfw
		glew
		assimp
		${OPENGL_LIBRARIES}
		${ASSIMP_LIBRARY}
	)
endif()

add_dependencies(Game glfw_ext assimp_ext)

