cmake_minimum_required(VERSION 3.10)

project(libgcc)

set(LIBS_NAME "c" "gcc" "m")
foreach(LIB ${LIBS_NAME})
    add_library(${LIB} STATIC IMPORTED GLOBAL)
    set_target_properties(${LIB} PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/lib/lib${LIB}.a")
    add_library("${PROJECT_NAME}::${LIB}" ALIAS ${LIB})
endforeach(LIB ${LIBS_NAME})
