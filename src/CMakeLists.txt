add_subdirectory(LynxEngine)
add_subdirectory(EngineModules)
add_subdirectory(Demo)

# No windows support
if(UNIX)
add_subdirectory(LynxEditor)
endif()