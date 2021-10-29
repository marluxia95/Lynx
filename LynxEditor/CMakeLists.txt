find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(LynxEditor
    src/main.cpp
)

target_link_libraries(LynxEditor 
    LynxEngine 
    Qt5::Widgets
)