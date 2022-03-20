cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 17)
project(Demo VERSION 0.1)

if(UNIX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lXrandr -lXext -lXrender -lX11 -lpthread -lxcb -lXau -lXdmcp -lGLEW -lGLU -lm -lGL -lglfw -lrt -lm -ldl")
endif()

if(BUILD_DEBUG)
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g" )
endif()

add_executable( 
    Demo demo.cpp
)

target_include_directories(Demo PRIVATE $(ENGINE))
target_include_directories(Demo PRIVATE ${LIBS}/imgui)
target_include_directories(Demo PRIVATE ${LIBS})

target_link_directories(Demo PRIVATE ${DEPS_INSTALL_DIR}/lib )

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

include_directories( 
    Demo

    ${SOURCE_DIR}/LynxEngine
	${SOURCE_DIR}/EngineModules
	${DEPS_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIRS}  
    ${GLEW_INCLUDE}
    ${GLFW_INCLUDE}    
    ${ASSIMP_INCLUDE}
    ${ENGINE}
    ${MODULES}
)

if(UNIX)
	target_link_libraries(
		Demo

		LynxEngine
		glfw
		assimp
		${GLEW}
		${OPENGL_LIBRARIES}
		${GLEW_LIBRARIES}
		${ASSIMP_LIBRARY}
	)
elseif(WIN32)
	target_link_libraries(
		Demo

		LynxEngine
		glfw
		glew
		assimp
		${OPENGL_LIBRARIES}
		${ASSIMP_LIBRARY}
	)
endif()

add_dependencies(Demo LynxEngine glfw assimp)
