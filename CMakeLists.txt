project(bn256)
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.1.0
)

FetchContent_MakeAvailable(Catch2)

FetchContent_Declare(
  span
  GIT_REPOSITORY "https://github.com/martinmoene/span-lite"
  GIT_TAG        v0.10.3
)

FetchContent_GetProperties(span)
if(NOT span_POPULATED)
  FetchContent_Populate(span)
endif()


find_package(Boost 1.78 REQUIRED)
add_subdirectory(src)
add_subdirectory(test)
