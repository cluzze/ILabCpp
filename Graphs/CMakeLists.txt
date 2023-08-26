cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME graphs)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-O2")

add_library(${PROJECT_NAME}_lib STATIC graph.hpp)
set_target_properties(${PROJECT_NAME}_lib PROPERTIES LINKER_LANGUAGE CXX)

find_package(GTest REQUIRED)
include_directories(${GTest_INCLUDE_DIRS} include)

add_executable(${PROJECT_NAME}_tests tests.cpp)
target_link_libraries(${PROJECT_NAME}_tests PRIVATE GTest::GTest GTest::Main ${PROJECT_NAME}_lib)

add_executable(${PROJECT_NAME} main.cpp)