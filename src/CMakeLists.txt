
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})
add_library (
        bn256
        lattice.cpp
        optate.cpp
        bn256.cpp
        random_256.cpp)
target_include_directories (bn256 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${span_SOURCE_DIR}/include)