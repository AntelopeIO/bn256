include(CheckCXXSourceCompiles)

check_cxx_source_compiles("
int main() {
  using T=_ExtInt(512);
  return 0;
}
"  HAVE_EXTINT)


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

if (BN256_ENABLE_BMI2)
        target_compile_options(bn256 PUBLIC -mbmi2)
        target_compile_definitions(bn256 PRIVATE BN256_HAS_BMI2)
endif()

if(BN256_INSTALL_COMPONENT)
   set(INSTALL_COMPONENT_ARGS COMPONENT ${BN256_INSTALL_COMPONENT} EXCLUDE_FROM_ALL)
endif()

install(TARGETS bn256
        LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR} ${INSTALL_COMPONENT_ARGS}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR} ${INSTALL_COMPONENT_ARGS})