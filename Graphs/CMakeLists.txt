cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME graphs)
project(${PROJECT_NAME} LANGUAGES CXX)

add_library(${PROJECT_NAME}_lib INTERFACE graph.hpp)

find_package(GTest REQUIRED)
include_directories(${GTest_INCLUDE_DIRS} include)

add_executable(${PROJECT_NAME}_tests tests.cpp)
target_link_libraries(${PROJECT_NAME}_tests PRIVATE GTest::GTest GTest::Main ${PROJECT_NAME}_lib)

add_executable(${PROJECT_NAME} main.cpp)