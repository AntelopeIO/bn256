include(CheckCXXSourceCompiles)

check_cxx_source_compiles("
int main() {
  using T=_ExtInt(512);
  return 0;
}
"  HAVE_EXTINT)


if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64" OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "amd64")
        include(CheckCXXSourceRuns)

        check_cxx_source_compiles("int main () { return __ELF__; }" IS_X86_64_ELF)

        if (NOT IS_X86_64_ELF)
                ## For ELF targets, we can use `__attribute__((ifunc("resolver")))` to resolve the function address at runtime
                ## based on whether the CPU suports BMI2. However, it is not possible for any other targets. We can only detect
                ## BMI2 at configuration time.
                set(CMAKE_REQUIRED_FLAGS -mbmi2)
                check_cxx_source_runs("int main ()
                {
                __builtin_cpu_init ();
                return (__builtin_cpu_supports (\"bmi2\")) == 0;
                }
                "
                HAVE_BMI2)
        endif()
endif()


add_library (
        bn256
        bn256.cpp
        random_255.cpp)
target_include_directories (bn256 PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>"
                                         "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../third-party/span/include>"
                                         "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")



if (HAVE_EXTINT)
        target_compile_definitions(bn256 PUBLIC BN256_HAS_EXTINT)
endif()


if (IS_X86_64_ELF OR HAVE_BMI2)
        target_compile_options(bn256 PUBLIC -mbmi2)
endif()

if(BN256_INSTALL_COMPONENT)
   set(INSTALL_COMPONENT_ARGS COMPONENT ${BN256_INSTALL_COMPONENT} EXCLUDE_FROM_ALL)
endif()

install(TARGETS bn256
        LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR} ${INSTALL_COMPONENT_ARGS}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR} ${INSTALL_COMPONENT_ARGS})