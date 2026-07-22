cmake_minimum_required(VERSION 3.16)
project(ethereum_offline_signer C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(SECP256K1_ENABLE_MODULE_RECOVERY ON CACHE BOOL "" FORCE)
set(SECP256K1_BUILD_BENCHMARK OFF CACHE BOOL "" FORCE)
set(SECP256K1_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SECP256K1_BUILD_EXHAUSTIVE_TESTS OFF CACHE BOOL "" FORCE)
set(SECP256K1_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/secp256k1/CMakeLists.txt")
  message(FATAL_ERROR "Missing vendor/secp256k1. Run ./setup.sh first.")
endif()

add_subdirectory(vendor/secp256k1 EXCLUDE_FROM_ALL)

add_executable(eth_signer src/eth_signer.c)
target_include_directories(eth_signer PRIVATE include)
target_link_libraries(eth_signer PRIVATE secp256k1)
target_compile_options(eth_signer PRIVATE -Wall -Wextra -Wpedantic -Werror)
