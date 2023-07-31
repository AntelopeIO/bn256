cmake_minimum_required(VERSION 3.12)

# set version and extract major version (currently version_major is 1).
set(version 1.1.0)
string(REPLACE "." ";" version_list ${version}) # transform into list
list(GET version_list 0 version_major)

project(bn256 VERSION ${version_major})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall)

option(BN256_ENABLE_TEST "BN256_ENABLE_TEST" ON)
option(BN256_ENABLE_BMI2 "enable bmi2 intruction set, only for supported x86-64 targets" OFF)

add_subdirectory(src)

if(BN256_ENABLE_TEST)
   enable_testing()
   add_subdirectory(third-party/Catch2)
   add_subdirectory(test)
endif()

include(GNUInstallDirs)
set_property(TARGET bn256 PROPERTY VERSION ${version})
set_property(TARGET bn256 PROPERTY SOVERSION ${version_major})
set_property(TARGET bn256 PROPERTY INTERFACE_bn256_MAJOR_VERSION ${version_major})
set_property(TARGET bn256 APPEND PROPERTY COMPATIBLE_INTERFACE_STRING bn256_MAJOR_VERSION)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY third-party/span/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/bn256)

install(TARGETS bn256
        EXPORT bn256
        LIBRARY
        ARCHIVE)

install(EXPORT bn256
        DESTINATION "lib/cmake/bn256"
        FILE bn256-targets.cmake)

include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/bn256-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bn256
)

install(FILES
          "${CMAKE_CURRENT_BINARY_DIR}/bn256-config.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/bn256-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bn256
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/bn256-config-version.cmake"
  VERSION "${version}"
  COMPATIBILITY AnyNewerVersion
)

export(TARGETS bn256 FILE "${CMAKE_CURRENT_BINARY_DIR}/bn256-targets.cmake")
