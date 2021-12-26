option(BUILD_EDITOR "Build editor" OFF)
option(BUILD_EXAMPLES "Build example projects" ON)

if(BUILD_EXAMPLES)
add_subdirectory(LynxEngine)

add_subdirectory(EngineModules)

add_subdirectory(Demo)
endif()

if(BUILD_EDITOR)
add_subdirectory(LynxEditor)
endif()