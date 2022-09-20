set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../src ${Boost_INCLUDE_DIRS})

add_executable (gfp_test gfp_test.cpp)
add_executable (lattice_test lattice_test.cpp)
add_executable (bn256_test bn256_test.cpp)
add_executable (main_test main_test.cpp)

target_link_libraries (gfp_test LINK_PUBLIC bn256)
target_link_libraries (lattice_test LINK_PUBLIC bn256)
target_link_libraries (bn256_test LINK_PUBLIC bn256)
target_link_libraries (main_test LINK_PUBLIC bn256)

enable_testing()

add_test(
        NAME gfp_test
        COMMAND $<TARGET_FILE:gfp_test>
)

add_test(
        NAME lattice_test
        COMMAND $<TARGET_FILE:lattice_test>
)

add_test(
        NAME bn256_test
        COMMAND $<TARGET_FILE:bn256_test>
)

add_test(
        NAME main_test
        COMMAND $<TARGET_FILE:main_test>
)