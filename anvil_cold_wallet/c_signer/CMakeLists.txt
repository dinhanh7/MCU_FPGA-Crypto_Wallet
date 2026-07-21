cmake_minimum_required(VERSION 3.16)
project(crypvault_mcu_router C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Đã loại bỏ hoàn toàn libsecp256k1 vì ký ECDSA được thực hiện trên FPGA
# add_subdirectory(vendor/secp256k1 EXCLUDE_FROM_ALL)

add_executable(eth_signer src/eth_signer.c)
target_include_directories(eth_signer PRIVATE include)

# Thêm define __linux__ khi biên dịch bằng CMake trên PC (để dùng chế độ Mock SPI)
target_compile_definitions(eth_signer PRIVATE __linux__)

target_compile_options(eth_signer PRIVATE -Wall -Wextra -Wpedantic -Werror)
