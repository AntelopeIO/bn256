function(add_test_executable TARGET)
   add_executable(${TARGET} ${ARGN})
   target_link_libraries(${TARGET} PRIVATE bn256)
   add_test(
         NAME ${TARGET}
         COMMAND ${TARGET}
   )
endfunction()

enable_testing()

add_test_executable(gfp_test gfp_test.cpp)
add_test_executable(lattice_test lattice_test.cpp)
add_test_executable(bn256_test bn256_test.cpp)
add_test_executable(main_test main_test.cpp)
add_test_executable(example_test example_test.cpp)