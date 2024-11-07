##
# @file toolchain_file.cmake
# @brief 
#/

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR Linux)

set(TOOLCHAIN_DIR "${PLATFORM_PATH}/toolchain/<your-toolchain-name>")
set(TOOLCHAIN_PRE "<your-toolchain-prefix>")

# set(TOOLCHAIN_INCLUDE
#     ${TOOLCHAIN_DIR}/include
#     )
# set(TOOLCHAIN_LIB
#     ${TOOLCHAIN_DIR}/lib/gcc
#     )

message(STATUS "[TOP] PLATFORM_PATH: ${PLATFORM_PATH}")

set(CMAKE_AR "${TOOLCHAIN_DIR}/bin/${TOOLCHAIN_PRE}ar")
set(CMAKE_C_COMPILER "${TOOLCHAIN_DIR}/bin/${TOOLCHAIN_PRE}gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_DIR}/bin/${TOOLCHAIN_PRE}g++")

SET (CMAKE_C_COMPILER_WORKS 1)
SET (CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_DIR}/bin)

set(CMAKE_C_FLAGS "<your-compiler-c-flags>")
