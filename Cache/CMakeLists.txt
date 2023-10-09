cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME cache)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(perfectcache_lib INTERFACE perfectcache.hpp)
add_library(lrucache_lib INTERFACE lrucache.hpp)
add_library(${PROJECT_NAME}_lib INTERFACE cache.hpp)

find_package(GTest REQUIRED)
include_directories(${GTest_INCLUDE_DIRS} include)

add_executable(${PROJECT_NAME}_tests tests.cpp)
target_link_libraries(${PROJECT_NAME}_tests PRIVATE GTest::GTest GTest::Main cache_lib)


add_executable(perfectcache perf_driver.cpp)

add_executable(${PROJECT_NAME} main.cpp)