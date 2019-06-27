cmake_minimum_required(VERSION 3.13.4)

project(JuliaExplorer)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

find_package(assimp REQUIRED)
if (assimp_FOUND)
    set(ASSIMP_LIBRARY "assimp")
    add_library(${ASSIMP_LIBRARY} SHARED IMPORTED)
    set_target_properties(${ASSIMP_LIBRARY} PROPERTIES IMPORTED_LOCATION "/usr/lib/x86_64-linux-gnu/libassimp.so")
endif(assimp_FOUND)

message(STATUS "${ASSIMP_LIBRARY} ${ASSIMP_LIBRARY_DIRS}")


set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB src_files "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.hpp")

set(SOURCES ${src_files})

#This is for glad, and I'm glad
add_library("glad" "${SRC_DIR}/glad.c")
target_include_directories("glad" PRIVATE "include")

add_executable(${PROJECT_NAME} ${SOURCES})

#Includes
target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}" ${ASSIMP_LIBRARY})


target_include_directories(${PROJECT_NAME} PRIVATE "./include" "/usr/include/stb" "${ASSIMP_INCLUDE_DIR}")