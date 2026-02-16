cmake_minimum_required(VERSION 3.25.0)
project(pman LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(pman_lib
			src/project_repository.cc
			src/project.cc
			src/project_manager.cc					
)
target_include_directories(pman_lib PUBLIC ${CMAKE_SOURCE_DIR}/src/include/)

add_executable(tests
				tests/test_repository.cpp
				tests/test_manager.cpp
)

target_link_libraries(tests PRIVATE pman_lib)


