SET(CMAKE_TOOLCHAIN_FILE ../toolchain/Prizm.cmake)

project(example1)

file(GLOB EXAMPLE1_SRC "src/example1/*.c*")

add_executable(example1 ${EXAMPLE1_SRC})

makeg3a(example1 "example" "unselected.bmp" "selected.bmp")