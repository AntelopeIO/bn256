include_directories(${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB BN256 "*.cpp")
add_library ( bn256 ${BN256} )
target_include_directories (bn256 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})