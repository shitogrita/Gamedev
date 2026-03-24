cmake_minimum_required(VERSION 3.23)
project(BrickGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)

option(COVERAGE "Enable LLVM coverage" ON)

if(COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    message(STATUS "LLVM Coverage enabled")
    add_compile_options(-fprofile-instr-generate -fcoverage-mapping -O0 -g)
    add_link_options(-fprofile-instr-generate -fcoverage-mapping)
endif()

find_package(Qt6 REQUIRED COMPONENTS Gui Widgets)
find_package(GTest REQUIRED)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})


add_subdirectory(gui)
add_subdirectory(brick_game)

add_executable(BrickGame_Desktop main_desktop.cpp)
target_link_libraries(BrickGame_Desktop PUBLIC GameWidget GameManager Snake Apple)




add_subdirectory(Test)

target_link_libraries(BrickGame_Desktop
        PRIVATE
        GameWidget
        GameManager Snake Apple
        TetrisLogic
        Qt::Widgets
        Qt::Gui
        Qt::Core
)

add_executable(BrickGame_CLI
        main_cli.c
)

target_link_libraries(BrickGame_CLI PRIVATE
        TetrisCli
        SnakeCli
        ${CURSES_LIBRARIES}
)


file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/*.c
        ${CMAKE_SOURCE_DIR}/*.cpp
        ${CMAKE_SOURCE_DIR}/*.h
        ${CMAKE_SOURCE_DIR}/*.hpp
        EXCLUDE cmake-build-*
)

add_custom_target(format
        COMMAND clang-format -i ${ALL_CXX_SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-format on all source files"
)

add_custom_target(format-check
        COMMAND clang-format -n ${ALL_CXX_SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Checking clang-format on all source files"
)
