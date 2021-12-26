file(COPY bin DESTINATION ${CMAKE_BINARY_DIR})

add_library(lua SHARED IMPORTED GLOBAL)
set_target_properties(lua PROPERTIES IMPORTED_IMPLIB "${DEPS_INCLUDE_DIR}/bin/lua.lib")

add_library(glfw SHARED IMPORTED GLOBAL)
set_target_properties(glfw PROPERTIES IMPORTED_IMPLIB "${DEPS_INCLUDE_DIR}/bin/glfw3dll.lib")

add_library(glew SHARED IMPORTED GLOBAL)
set_target_properties(glew PROPERTIES IMPORTED_IMPLIB "${DEPS_INCLUDE_DIR}/bin/glew32.lib")

add_library(assimp SHARED IMPORTED GLOBAL)
set_target_properties(assimp PROPERTIES IMPORTED_IMPLIB "${DEPS_INCLUDE_DIR}/bin/assimp-vc142-mtd.lib")