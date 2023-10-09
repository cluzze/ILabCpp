cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME matrix)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(${PROJECT_NAME} LANGUAGES CXX)

add_library(vector_lib INTERFACE vector.hpp)
add_library(${PROJECT_NAME}_lib INTERFACE matrix.hpp)

find_package(GTest REQUIRED)
include_directories(${GTest_INCLUDE_DIRS} include)

add_executable(vector_tests vector_tests.cpp)
target_link_libraries(vector_tests PRIVATE GTest::GTest GTest::Main vector_lib)

add_executable(${PROJECT_NAME}_tests matrix_tests.cpp)
target_link_libraries(${PROJECT_NAME}_tests PRIVATE GTest::GTest GTest::Main vector_lib)

add_executable(${PROJECT_NAME} main.cpp)