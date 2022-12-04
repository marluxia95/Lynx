include(ExternalProject)

if(WIN32)
	file(GLOB libs "lib/win32")
	file(COPY bin DESTINATION ${CMAKE_BINARY_DIR})
	file(COPY include DESTINATION ${CMAKE_BINARY_DIR})
	file(COPY ${libs} DESTINATION ${CMAKE_BINARY_DIR})
endif()

option(PREBUILT_DEPENDENCIES "Do not download and build dependencies, just use prebuilt ones" OFF)

if(PREBUILT_DEPENDENCIES)
	return()
endif()

if(UNIX)
  externalproject_add(lua_ext
	SOURCE_DIR ${DEPS_INCLUDE_DIR}/lua
	INSTALL_COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/src/liblua.a ${DEPS_INSTALL_DIR}/lib/liblua.a
	COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/src <SOURCE_DIR>
        BUILD_COMMAND ${CMAKE_COMMAND} -E env
            HOST_CC=...
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

# zlib not used for now
ExternalProject_Add(
  zlib_ext
  PREFIX ${DEPS_INCLUDE_DIR}/zlib
  CMAKE_ARGS  -DBUILD_SHARED_LIBS=OFF
              -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
  UPDATE_COMMAND ""
  #INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
  #                <SOURCE_DIR> ${DEPS_INSTALL_DIR}/include/zlib
  UPDATE_DISCONNECTED 1
  URL https://codeload.github.com/madler/zlib/zip/refs/heads/master
)

ExternalProject_Add(
  gli_ext
  PREFIX ${DEPS_INCLUDE_DIR}/gli
  INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
                  <SOURCE_DIR>/gli ${DEPS_INSTALL_DIR}/include/gli
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  UPDATE_COMMAND ""
  UPDATE_DISCONNECTED 1
  URL https://codeload.github.com/g-truc/gli/zip/refs/heads/master
)

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
	      -DBUILD_SHARED_LIBS=OFF
              -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
  UPDATE_COMMAND ""
  UPDATE_DISCONNECTED 1
  URL https://github.com/glfw/glfw/releases/download/3.3.4/glfw-3.3.4.zip
)

ExternalProject_Add(
  assimp_ext
  PREFIX ${DEPS_INCLUDE_DIR}/assimp
  CMAKE_ARGS  -DBUILD_SHARED_LIBS=OFF
              -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
              -DASSIMP_NO_EXPORT=ON
              -DASSIMP_BUILD_ASSIMP_TOOLS=OFF
              -DASSIMP_BUILD_TESTS=OFF
              -DASSIMP_INSTALL_PDB=OFF
	      -DASSIMP_BUILD_ZLIB=OFF
	      -DASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT=OFF
	      -DASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT=OFF
	      -DASSIMP_BUILD_FBX_IMPORTER=ON
	      -DASSIMP_BUILD_OBJ_IMPORTER=ON
	      -DASSIMP_BUILD_ASSIMP_VIEW=OFF
  UPDATE_COMMAND ""
  UPDATE_DISCONNECTED 1
  URL https://github.com/assimp/assimp/archive/refs/tags/v5.0.1.zip
)

ExternalProject_Add(
  stb_ext
  PREFIX ${DEPS_INCLUDE_DIR}/stb
  URL https://github.com/nothings/stb/archive/refs/heads/master.zip
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
                        <SOURCE_DIR> ${DEPS_INSTALL_DIR}/include/stb
)

add_library(lua    STATIC IMPORTED GLOBAL)
add_library(glfw   STATIC IMPORTED GLOBAL)
add_library(glew   STATIC IMPORTED GLOBAL)
add_library(assimp STATIC IMPORTED GLOBAL)
add_library(zlib   STATIC IMPORTED GLOBAL)

if(WIN32)
	set_target_properties(lua    PROPERTIES IMPORTED_IMPLIB "${DEPS_INSTALL_DIR}/lib/lua.lib")
	set_target_properties(glfw3  PROPERTIES IMPORTED_IMPLIB "${DEPS_INSTALL_DIR}/lib/glfw3dll.lib")
	set_target_properties(glew   PROPERTIES IMPORTED_IMPLIB "${DEPS_INSTALL_DIR}/lib/glew32.lib")
	set_target_properties(assimp PROPERTIES IMPORTED_IMPLIB "${DEPS_INSTALL_DIR}/lib/assimp-vc142-mtd.lib")
elseif(UNIX)
	set_target_properties(lua    PROPERTIES IMPORTED_LOCATION "${DEPS_INSTALL_DIR}/lib/liblua.a")
	set_target_properties(glfw   PROPERTIES IMPORTED_LOCATION "${DEPS_INSTALL_DIR}/lib64/libglfw3.a")
	find_package(GLEW REQUIRED)
	set_target_properties(assimp PROPERTIES IMPORTED_LOCATION "${DEPS_INSTALL_DIR}/lib/libassimp.a")
	set_target_properties(zlib   PROPERTIES IMPORTED_LOCATION "${DEPS_INSTALL_DIR}/lib/libz.a")
endif()
