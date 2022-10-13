project(bn256)
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall)

find_package(Boost REQUIRED)

add_subdirectory(src)

option(BN256_ENABLE_TEST "BN256_ENABLE_TEST" ON)
if (BN256_ENABLE_TEST)
   enable_testing()
   add_subdirectory(third-party/Catch2)
   add_subdirectory(test)
endif ()
