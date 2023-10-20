cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME matrix)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(${PROJECT_NAME} LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "-g")

include_directories(include)

add_library(vector_lib INTERFACE include/vector.hpp)
add_library(matrix_chain_lib INTERFACE include/matrix_chain.hpp)
add_library(${PROJECT_NAME}_lib INTERFACE include/matrix.hpp)

find_package(GTest REQUIRED)
include_directories(${GTest_INCLUDE_DIRS} include)

add_executable(vector_tests vector_tests.cpp)
target_link_libraries(vector_tests PRIVATE GTest::GTest GTest::Main vector_lib)

add_executable(${PROJECT_NAME}_tests matrix_tests.cpp)
target_link_libraries(${PROJECT_NAME}_tests PRIVATE GTest::GTest GTest::Main vector_lib)

add_executable(matrix_chain_tests matrix_chain_tests.cpp)
target_link_libraries(matrix_chain_tests PRIVATE GTest::GTest GTest::Main matrix_chain_lib)

add_executable(${PROJECT_NAME} drivers/main.cpp)
add_executable(matrix_chain drivers/main2.cpp)