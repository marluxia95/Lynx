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

file(GLOB_RECURSE HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.hpp
)

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -fPIC" )

link_directories( ${DEPS_INSTALL_DIR} ${DEPS_INSTALL_DIR}/bin ${DEPS_INSTALL_DIR}/lib )

add_library( 
    LynxEngine STATIC ${SOURCES} ${HEADERS}
)

target_include_directories(LynxEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(LynxEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../lib/imgui)
target_include_directories(LynxEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
target_include_directories(LynxEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../build/include)

find_package(OpenGL REQUIRED)

if(UNIX)
    find_package(GLEW REQUIRED)
    if (GLEW_FOUND)
        include_directories(${GLEW_INCLUDE_DIRS})
        link_libraries(${GLEW_LIBRARIES})
    endif()
elseif(WIN32)
    add_library(glew STATIC IMPORTED GLOBAL)
	set_target_properties(glew PROPERTIES IMPORTED_LOCATION 
	"${CMAKE_BINARY_DIR}/lib/glew32.lib"
	)
endif()

include_directories( 
    LynxEngine

    ${CMAKE_BINARY_DIR}/include
    ${OPENGL_INCLUDE_DIRS}  
    ${GLFW_INCLUDE}    
    ${ASSIMP_INCLUDE}
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ 
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/glm 
)

if(UNIX)
	target_link_libraries(
		LynxEngine

		lua
		glfw
		assimp
		${OPENGL_LIBRARIES}
		${GLEW}
		${ASSIMP_LIBRARY}
	)
elseif(WIN32)
	target_link_libraries(
		LynxEngine

		lua
		glew
		glfw
		assimp
		${OPENGL_LIBRARIES}
		${ASSIMP_LIBRARY}
	)
endif()

add_dependencies(LynxEngine glfw_ext assimp_ext)