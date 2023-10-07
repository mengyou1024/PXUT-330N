function(auto_add_subdirecory directory)
    file(GLOB THIRDS_PATH "${directory}/*")
    foreach(THIRD ${THIRDS_PATH})
        if (IS_DIRECTORY "${THIRD}" AND EXISTS "${THIRD}/CMakeLists.txt")
            file(RELATIVE_PATH THIRD_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${THIRD})
            add_subdirectory(${THIRD_PATH})
        endif(IS_DIRECTORY "${THIRD}" AND EXISTS "${THIRD}/CMakeLists.txt")
    endforeach(THIRD ${THIRD_PATH})
endfunction(auto_add_subdirecory directory)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_COMPILER_PREFIX "arm-none-eabi-")

find_program(CMAKE_C_COMPILER NAMES ${CMAKE_COMPILER_PREFIX}gcc)
find_program(CMAKE_CXX_COMPILER NAMES ${CMAKE_COMPILER_PREFIX}g++)
find_program(CMAKE_ASM_COMPILER NAMES ${CMAKE_COMPILER_PREFIX}gcc)
find_program(CMAKE_OBJCOPY NAMES ${CMAKE_COMPILER_PREFIX}objcopy)

set(CMAKE_EXECUTABLE_SUFFIX_C   .elf)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)
set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf)

# set(CMAKE_C_FLAGS "-Wall -ffunction-sections -g -O2")
# set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
# set(CMAKE_ASM_FLAGS "-Wall -g -O2")
# set(CMAKE_EXE_LINKER_FLAGS "-g -O2 -nostartfiles")

set(CMAKE_C_FLAGS "-w -ffunction-sections -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_ASM_FLAGS "-w -O2")
set(CMAKE_EXE_LINKER_FLAGS "-O2 -nostartfiles")

auto_add_subdirecory("${CMAKE_SOURCE_DIR}/components")
message(STATUS ARCH:PXUT-330N)

