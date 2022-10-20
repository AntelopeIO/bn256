include(CheckCXXSourceCompiles)

check_cxx_source_compiles("
int main() {
  using T=_ExtInt(512);
  return 0;
}
"  HAVE_EXTINT)

add_library (
        bn256
        lattice.cpp
        optate.cpp
        bn256.cpp
        random_256.cpp)
target_include_directories (bn256 PUBLIC ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/../include ../third-party/span/include)

if (HAVE_EXTINT)
        target_compile_definitions(bn256 PUBLIC BN256_HAS_EXTINT)
endif()