cmake_minimum_required(VERSION 3.2.0)
project(Lynx VERSION 0.0.3)

set(CMAKE_CXX_STANDARD 20 )

set(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE_DIR ${BASE_DIR}/src)
set(DEPS_INSTALL_DIR ${CMAKE_BINARY_DIR})
set(DEPS_INCLUDE_DIR ${CMAKE_BINARY_DIR}/include)
set(DEPS_MAIN_DIR ${CMAKE_CURRENT_LIST_DIR}/deps)

if(UNIX)
	set(CMAKE_CXX_FLAGS "-lglfw3 -lGL -lGLEW -lX11 -lpthread -lXrandr -ldl -lassimp")
elseif(WIN32)
	cmake_policy(SET CMP0111 NEW)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

link_directories( ${CMAKE_BINARY_DIR} )

INSTALL(CODE "execute_process( \
    COMMAND ${CMAKE_COMMAND} -E create_symlink \
    ${BASE_DIR}/res   \
    ${DEPS_INSTALL_DIR}/res \
    )"
)

add_subdirectory(deps)

add_subdirectory(src)
