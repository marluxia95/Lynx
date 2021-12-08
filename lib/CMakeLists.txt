include(ExternalProject)

set(DEPS_INSTALL_DIR ${CMAKE_BINARY_DIR})
set(DEPS_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR})

if(UNIX)
externalproject_add(lua_ext
    PREFIX ${DEPS_INCLUDE_DIR}/lua
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
                    <SOURCE_DIR>/src ${DEPS_INSTALL_DIR}/include/lua
    BUILD_COMMAND ${CMAKE_COMMAND} -E env
        HOST_CC=...   # See below
        CC=${CMAKE_C_COMPILER}
        make CFLAGS="-fPIC" -C <SOURCE_DIR>
    CONFIGURE_COMMAND ""
    BUILD_IN_SOURCE TRUE
    URL https://www.lua.org/ftp/lua-5.4.3.tar.gz
)
elseif(WIN32)
externalproject_add(lua_ext
    PREFIX ${DEPS_INCLUDE_DIR}/lua
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
                    <SOURCE_DIR>/src ${DEPS_INSTALL_DIR}/include/lua
    BUILD_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_IN_SOURCE TRUE
    URL https://www.lua.org/ftp/lua-5.4.3.tar.gz
)
endif()

ExternalProject_Add(
  glm_ext
  PREFIX ${DEPS_INCLUDE_DIR}/glm
  INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
                  <SOURCE_DIR>/glm ${DEPS_INSTALL_DIR}/include/glm
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  UPDATE_COMMAND ""
  UPDATE_DISCONNECTED 1
  URL https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip
)

ExternalProject_Add(
  glfw_ext
  PREFIX ${DEPS_INCLUDE_DIR}/glfw
  CMAKE_ARGS  -DGLFW_BUILD_TESTS=OFF
              -DGLFW_BUILD_EXAMPLES=OFF
              -DGLFW_BUILD_DOCS=OFF
              -DGLFW_INSTALL=ON
              -DBUILD_SHARED_LIBS=ON
              -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
  UPDATE_COMMAND ""
  UPDATE_DISCONNECTED 1
  URL https://github.com/glfw/glfw/releases/download/3.3.4/glfw-3.3.4.zip
)

ExternalProject_Add(
  assimp_ext
  PREFIX ${DEPS_INCLUDE_DIR}/assimp
  CMAKE_ARGS  -DGLFW_BUILD_TESTS=OFF
              -DGLFW_BUILD_EXAMPLES=OFF
              -DGLFW_BUILD_DOCS=OFF
              -DGLFW_INSTALL=ON
              -DBUILD_SHARED_LIBS=ON
              -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
  UPDATE_COMMAND ""
  UPDATE_DISCONNECTED 1
  URL https://github.com/assimp/assimp/archive/refs/tags/v5.0.1.zip
)

ExternalProject_Add(
  stb_image_ext
  PREFIX ${DEPS_INCLUDE_DIR}/stb
  URL https://github.com/nothings/stb/archive/refs/heads/master.zip
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)


if(UNIX)
add_library(lua STATIC IMPORTED GLOBAL)
set_target_properties(lua PROPERTIES IMPORTED_LOCATION
  "${DEPS_INSTALL_DIR}/lib/${CMAKE_STATIC_MODULE_PREFIX}liblua${CMAKE_STATIC_LIBRARY_SUFFIX}"
)
elseif(WIN32)
add_library(lua SHARED IMPORTED GLOBAL)
set_target_properties(lua PROPERTIES IMPORTED_LOCATION
  "${DEPS_INSTALL_DIR}/lib/lua54.dll"
)
set_target_properties(lua PROPERTIES LOCATION
  "${DEPS_INSTALL_DIR}/lib/lua54.dll"
)
endif()


add_library(glfw SHARED IMPORTED GLOBAL)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION
  "${DEPS_INSTALL_DIR}/lib/${CMAKE_SHARED_MODULE_PREFIX}glfw${CMAKE_SHARED_LIBRARY_SUFFIX}"
)

add_library(assimp SHARED IMPORTED GLOBAL)
set_target_properties(assimp PROPERTIES IMPORTED_LOCATION
  "${DEPS_INSTALL_DIR}/lib/${CMAKE_SHARED_MODULE_PREFIX}assimp${CMAKE_SHARED_LIBRARY_SUFFIX}"
)





