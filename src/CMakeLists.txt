include(CheckCXXSourceCompiles)

check_cxx_source_compiles("
int main() {
  using T=_ExtInt(512);
  return 0;
}
"  HAVE_EXTINT)


if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64" OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "amd64")
include(CheckCXXSourceRuns)
set(CMAKE_REQUIRED_FLAGS -mbmi2)
check_cxx_source_runs("int main ()
{
  __builtin_cpu_init ();
  return (__builtin_cpu_supports (\"bmi2\")) == 0;
}
"
 HAVE_BMI2)
endif()


add_library (
        bn256
        # lattice.cpp
        # optate.cpp
        bn256.cpp
        random_256.cpp)
target_include_directories (bn256 PUBLIC ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/../include ../third-party/span/include)
target_link_libraries(bn256 PUBLIC OpenSSL::Crypto)

if (HAVE_EXTINT)
        target_compile_definitions(bn256 PUBLIC BN256_HAS_EXTINT)
endif()

if (HAVE_BMI2)
        target_compile_options(bn256 PUBLIC -mbmi2)
endif()
