cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME triangles)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-g")

add_library(${PROJECT_NAME}_lib INTERFACE triangles.hpp)
add_library(broadphase_lib INTERFACE broadphase.hpp)

find_package(GTest REQUIRED)
include_directories(${GTest_INCLUDE_DIRS} include)

add_executable(${PROJECT_NAME}_tests tests.cpp)
target_link_libraries(${PROJECT_NAME}_tests PRIVATE GTest::GTest GTest::Main ${PROJECT_NAME}_lib)

add_executable(${PROJECT_NAME} main.cpp)