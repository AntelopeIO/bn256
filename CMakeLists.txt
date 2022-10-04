cmake_minimum_required(VERSION 3.8)
project(bn256)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Boost)
add_subdirectory(src)
add_subdirectory(test)


