cmake_minimum_required(VERSION 3.25.0)
project(pman LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(pman_lib
			src/project_repository.cc
			src/project.cc
			src/project_manager.cc
			src/ui_components.cc
)
target_include_directories(pman_lib PUBLIC ${CMAKE_SOURCE_DIR}/src/include/)

add_executable(tests
				tests/test_repository.cpp
				tests/test_manager.cpp
)

target_link_libraries(tests PRIVATE pman_lib)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FTXUI REQUIRED ftxui)

add_executable(pman
    src/pman.cc
)

target_include_directories(pman PRIVATE ${FTXUI_INCLUDE_DIRS})
target_link_libraries(pman PRIVATE pman_lib ${FTXUI_LIBRARIES})
target_compile_options(pman PRIVATE ${FTXUI_CFLAGS_OTHER})


