cmake_minimum_required(VERSION 2.6)

SET(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/../../toolchain/Prizm.cmake)

project(example1)

file(GLOB EXAMPLE1_SRC "src/example1/*.c*")

add_executable(example1 ${EXAMPLE1_SRC})

makeg3a("${CMAKE_BINARY_DIR}/example1" "example" "${CMAKE_SOURCE_DIR}/unselected.bmp" "${CMAKE_SOURCE_DIR}/selected.bmp")
