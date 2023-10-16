cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(PXUT-330N)

project("PXUT-330N" LANGUAGES C ASM CXX)

include_directories(".")

set(
    PXUT_SRC
    startup.S
    globe.c
    main.c
    common.c
    display.c
    menu.c
    sysdef.c
    ini.c
    gain.c
    SDcard.c
    armtest.c
    tools.c
    test.c
    image_bin.c
    character_bin.c
    music_bin.c
)
message(STATUS ${PXUT_SRC})
add_executable(${PROJECT_NAME}  ${PXUT_SRC})
target_include_directories(${PROJECT_NAME}  PUBLIC ".")
target_link_libraries(${PROJECT_NAME}  PRIVATE fatfs::fatfs m10era::m10era libgcc::c libgcc::m libgcc::gcc)
target_link_options(${PROJECT_NAME}  PRIVATE -T "${CMAKE_SOURCE_DIR}/link/ldscript.lds")
target_precompile_headers(${PROJECT_NAME} PUBLIC "gch.h")


add_custom_command(
    TARGET ${PROJECT_NAME}  POST_BUILD 
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
)

add_custom_target(
    size ALL
    COMMAND ${CMAKE_SIZE} "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf"
    DEPENDS ${PROJECT_NAME}
)

add_custom_target(
    flash 
    COMMAND "${CMAKE_SOURCE_DIR}/tools/M10BurnUser" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin" 
    DEPENDS size
)
