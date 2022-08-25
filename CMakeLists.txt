cmake_minimum_required(VERSION 3.8)
project(bn256)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

include_directories(src ${Boost_INCLUDE_DIRS})

add_executable(gfp_test src/gfp_test.cpp src/gfp.cpp)
