cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(PXUT-330N)

project("PXUT-330N" LANGUAGES C ASM CXX)

include_directories(".")
# aux_source_directory("." PXUT_SRC)
set(
    PXUT_SRC
    startup.s
    main.c
    common.c
    display.c
    sysdef.c
    ini.c
    gain.c
    sdcard.c
    armtest.c
    tools.c
)
message(STATUS ${PXUT_SRC})
add_executable(${PROJECT_NAME}  ${PXUT_SRC})
target_include_directories(${PROJECT_NAME}  PUBLIC ".")
target_link_libraries(${PROJECT_NAME}  PRIVATE fatfs::fatfs m10era::m10era libgcc::c libgcc::m libgcc::gcc ) #"-T${CMAKE_SOURCE_DIR}/link/ldscript.lds"
target_link_options(${PROJECT_NAME}  PRIVATE -T "${CMAKE_SOURCE_DIR}/link/ldscript.lds")

add_custom_command(
    TARGET ${PROJECT_NAME}  POST_BUILD 
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
)

add_custom_target(flash ALL COMMAND "${CMAKE_SOURCE_DIR}/tools/M10BurnUser" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin" DEPENDS ${PROJECT_NAME})
