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
        target_compile_definitions(bn256 PUBLIC HAVE_EXTINT)
else()
        target_sources(bn256 PRIVATE gfp_mul_amd64.s)
        set_property ( SOURCE gfp_mul_amd64.s PROPERTY LANGUAGE ASM )
        if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
                target_sources(bn256 PRIVATE gfp_amd64.s)
                set_property ( SOURCE gfp_amd64.s PROPERTY LANGUAGE ASM )
        endif()
endif()