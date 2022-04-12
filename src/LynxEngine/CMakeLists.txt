file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/**/*.c
)

file(GLOB_RECURSE HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/**/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/**/*.hpp
)

if(UNIX)
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIC" )
endif()

if(BUILD_DEBUG)
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g" )
endif()

link_directories( ${DEPS_INSTALL_DIR} ${DEPS_INSTALL_DIR}/bin ${DEPS_INSTALL_DIR}/lib )

add_library( 
    LynxEngine SHARED ${SOURCES} ${HEADERS}
)

target_precompile_headers(LynxEngine PRIVATE "lynx.h")

target_include_directories(LynxEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(OpenGL REQUIRED)

include_directories( 
    LynxEngine

    ${CMAKE_BINARY_DIR}
	${DEPS_INCLUDE_DIR}
	${DEPS_MAIN_DIR}
    ${OPENGL_INCLUDE_DIRS}  
    ${GLFW_INCLUDE}    
    ${ASSIMP_INCLUDE}
)

if(UNIX)
	target_link_libraries(
		LynxEngine

		lua
		glfw
		assimp
		zlib
		${OPENGL_LIBRARIES}
		${GLEW}
		${ASSIMP_LIBRARY}
	)
	add_dependencies(LynxEngine lua glfw_ext assimp_ext)
elseif(WIN32)
	target_link_libraries(
		LynxEngine

		lua
		glew
		glfw
		assimp
		zlib
		${OPENGL_LIBRARIES}
		${ASSIMP_LIBRARY}
	)
	add_dependencies(LynxEngine lua glfw_ext assimp_ext glew)
endif()

