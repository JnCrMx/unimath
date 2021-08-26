cmake_minimum_required (VERSION 3.4)

project(unimath)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 

file(GLOB_RECURSE sources src/**.cpp)

add_library(unimath SHARED ${sources})
target_include_directories(unimath PUBLIC include/)

add_executable(basic_test test/test.cpp)
target_link_libraries(basic_test PRIVATE unimath)

add_executable(basic_test2 test/test2.cpp)
target_link_libraries(basic_test2 PRIVATE unimath)

add_executable(basic_test3 test/test3.cpp)
target_link_libraries(basic_test3 PRIVATE unimath)
