cmake_minimum_required (VERSION 3.4)

project(unimath)

find_package(Threads)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 

file(GLOB_RECURSE sources src/**.cpp)

add_library(unimath SHARED ${sources})
target_include_directories(unimath PUBLIC include/)
target_link_libraries(unimath PUBLIC Threads::Threads)

add_executable(basic_test test/test.cpp)
target_link_libraries(basic_test PRIVATE unimath)

add_executable(basic_test2 test/test2.cpp)
target_link_libraries(basic_test2 PRIVATE unimath)

add_executable(basic_test3 test/test3.cpp)
target_link_libraries(basic_test3 PRIVATE unimath)

add_executable(bisection test/bisection.cpp)
target_link_libraries(bisection PRIVATE unimath)

add_executable(test4 test/test4.cpp)
target_link_libraries(test4 PRIVATE unimath)

add_executable(nodal_test test/nodal_test.cpp)
target_link_libraries(nodal_test PRIVATE unimath)
