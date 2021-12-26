cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 17)
project(TestModule VERSION 0.1)

file(GLOB_RECURSE SOURCES
    testModule.cpp
)

add_library( 
    TestModule SHARED ${SOURCES}
)

target_include_directories(TestModule PRIVATE ${SOURCE_DIR}/LynxEngine)
target_include_directories(TestModule PRIVATE ${LIBS}/imgui)
target_include_directories(TestModule PRIVATE ${LIBS})


target_link_libraries(
	TestModule

	LynxEngine
)

add_dependencies(TestModule LynxEngine)