add_library (
        bn256
        lattice.cpp
        optate.cpp
        bn256.cpp
        random_256.cpp)
target_include_directories (bn256 PUBLIC ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/../include ../third-party/span/include)