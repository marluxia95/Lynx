include(ExternalProject)

set(DEPS_INSTALL_DIR ${CMAKE_BINARY_DIR})
set(DEPS_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR})

externalproject_add(glm_ext
    PREFIX ${DEPS_INCLUDE_DIR}/glm
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
                    <SOURCE_DIR>/glm ${DEPS_INSTALL_DIR}/glm
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    UPDATE_COMMAND ""
    UPDATE_DISCONNECTED 1
    URL https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip
)

externalproject_add(glfw_ext
    PREFIX ${DEPS_INCLUDE_DIR}/glfw
    CMAKE_ARGS -DGLFW_BUILD_TESTS=OFF
               -DGLFW_BUILD_EXAMPLES=OFF
               -DGLFW_BUILD_DOCS=OFF
               -DGLFW_INSTALL=ON
               -DBUILD_SHARED_LIBS=ON
               -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
    UPDATE_COMMAND ""
    UPDATE_DISCONNECTED 1
    URL https://github.com/glfw/glfw/releases/download/3.3.4/glfw-3.3.4.zip
)

externalproject_add(assimp_ext
    PREFIX ${DEPS_INCLUDE_DIR}/assimp
    CMAKE_ARGS -DGLFW_BUILD_TESTS=OFF
               -DGLFW_BUILD_EXAMPLES=OFF
               -DGLFW_BUILD_DOCS=OFF
               -DGLFW_INSTALL=ON
               -DBUILD_SHARED_LIBS=ON
               -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
    UPDATE_COMMAND ""
    UPDATE_DISCONNECTED 1
    URL https://github.com/assimp/assimp/archive/refs/tags/v5.0.1.zip
)

IF(UNIX)
add_library(glfw STATIC IMPORTED GLOBAL)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION
  "${DEPS_INSTALL_DIR}/lib/${CMAKE_STATIC_MODULE_PREFIX}libglfw${CMAKE_SHARED_LIBRARY_SUFFIX}"
)

add_library(assimp STATIC IMPORTED GLOBAL)
set_target_properties(assimp PROPERTIES IMPORTED_LOCATION
  "${DEPS_INSTALL_DIR}/lib/${CMAKE_STATIC_MODULE_PREFIX}libassimp${CMAKE_SHARED_LIBRARY_SUFFIX}"
)
ENDIF(UNIX)

IF(WIN32)
add_library(glfw STATIC IMPORTED GLOBAL)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION
  "${DEPS_INSTALL_DIR}/lib/${CMAKE_STATIC_MODULE_PREFIX}glfw${CMAKE_SHARED_LIBRARY_SUFFIX}"
)

add_library(assimp STATIC IMPORTED GLOBAL)
set_target_properties(assimp PROPERTIES IMPORTED_LOCATION
  "${DEPS_INSTALL_DIR}/lib/${CMAKE_STATIC_MODULE_PREFIX}assimp${CMAKE_SHARED_LIBRARY_SUFFIX}"
)
ENDIF(WIN32)



