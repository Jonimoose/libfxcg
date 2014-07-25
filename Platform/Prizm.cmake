SET(PRIZM 1)

# SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS ${CMAKE_EXE_LINKER_FLAGS})
# SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS ${CMAKE_EXE_LINKER_FLAGS})
SET(CMAKE_EXECUTABLE_SUFFIX ".bin")
SET(CMAKE_C_OUTPUT_EXTENSION ".o")
SET(CMAKE_CXX_OUTPUT_EXTENSION ".o")

LINK_LIBRARIES(c)

SET(CMAKE_C_FLAGS "-Os -Wall -mb  -mhitachi -nostdlib -ffunction-sections -fdata-sections -I${CMAKE_CURRENT_SOURCE_DIR}/../include")
SET(CMAKE_CXX_FLAGS "-Os -Wall -mb  -mhitachi -nostdlib -ffunction-sections -fdata-sections -I${CMAKE_CURRENT_SOURCE_DIR}/../include")

SET(CMAKE_EXE_LINKER_FLAGS "-L../lib -T../toolchain/prizm.x -Wl,-static -Wl,-gc-sections -nostdlib")