
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})
add_library (
        bn256
        gfp.cpp
        gfp2.cpp
        gfp6.cpp
        gfp12.cpp
        gfp_generic.cpp
        bitlen.cpp
        lattice.cpp
        curve.cpp
        twist.cpp
        optate.cpp
        bn256.cpp
        random_256.cpp)
target_include_directories (bn256 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})