add_executable (gfp_test gfp_test.cpp)
add_executable (lattice_test lattice_test.cpp)
add_executable (bn256_test bn256_test.cpp)
add_executable (main_test main_test.cpp)
add_executable (example_test example_test.cpp)

target_link_libraries (gfp_test PUBLIC bn256)
target_link_libraries (lattice_test PUBLIC bn256)
target_link_libraries (bn256_test PUBLIC bn256)
target_link_libraries (main_test PUBLIC bn256)
target_link_libraries (example_test PUBLIC bn256)

enable_testing()

add_test(
        NAME gfp_test
        COMMAND gfp_test
)

add_test(
        NAME lattice_test
        COMMAND lattice_test
)

add_test(
        NAME bn256_test
        COMMAND bn256_test
)

add_test(
        NAME main_test
        COMMAND main_test
)

add_test(
        NAME example_test
        COMMAND example_test
)