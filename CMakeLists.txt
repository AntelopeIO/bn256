project(bn256)
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall)

option(BN256_ENABLE_TEST "BN256_ENABLE_TEST" ON)

# add defaults for openssl
if(NOT OPENSSL_ROOT_DIR AND APPLE AND UNIX AND "${OPENSSL_ROOT_DIR}" STREQUAL "")
   if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
      set(OPENSSL_ROOT_DIR "/opt/homebrew/opt/openssl@3;/opt/homebrew/opt/openssl@1.1")
   else()
      set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl@3;/usr/local/opt/openssl@1.1")
   endif()
endif()

find_package(OpenSSL REQUIRED)

add_subdirectory(src)

if(BN256_ENABLE_TEST)
   enable_testing()
   add_subdirectory(third-party/Catch2)
   add_subdirectory(test)
endif()
