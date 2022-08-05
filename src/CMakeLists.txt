option(BUILD_EDITOR "Build editor" OFF)
option(BUILD_EXAMPLES "Build example projects" ON)
option(BUILD_DEBUG "Enable build debugging (-g)" OFF)

if(BUILD_EXAMPLES)
add_subdirectory(LynxEngine)
add_subdirectory(Demo)
endif()

if(BUILD_EDITOR)
add_subdirectory(LynxEditor)
endif()
