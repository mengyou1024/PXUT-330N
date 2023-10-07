cmake_minimum_required(VERSION 3.10)

project(m10era)

add_library(m10era_private src/m10era_variable.c)

add_library(${PROJECT_NAME} STATIC IMPORTED GLOBAL)
target_link_libraries(${PROJECT_NAME} INTERFACE m10era_private)
set_target_properties(${PROJECT_NAME}  PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/lib/lib${PROJECT_NAME}.a")
target_include_directories(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")
add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS ${PROJECT_NAME})


