if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64" OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "amd64")
        include(CheckSymbolExists)
        check_symbol_exists(__ELF__ "stdio.h" IS_ELF_AMD64)
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

if(IS_ELF_AMD64)
        target_sources(bn256 PRIVATE mul_u256_bmi2.cpp mul_u256_nonconstexpr.cpp)
        set_source_files_properties(mul_u256_bmi2.cpp PROPERTIES COMPILE_FLAGS -mbmi2)
endif()

if(BN256_INSTALL_COMPONENT)
   set(INSTALL_COMPONENT_ARGS COMPONENT ${BN256_INSTALL_COMPONENT} EXCLUDE_FROM_ALL)
endif()

install(TARGETS bn256
        LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR} ${INSTALL_COMPONENT_ARGS}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR} ${INSTALL_COMPONENT_ARGS})