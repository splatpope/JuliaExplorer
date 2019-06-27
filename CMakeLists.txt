cmake_minimum_required(VERSION 3.13.4)

project(JuliaExplorer)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB src_files "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.hpp")

set(SOURCES ${src_files})

#This is for glad, and I'm glad
add_library("glad" "${SRC_DIR}/glad.c")
target_include_directories("glad" PRIVATE "include")

add_executable(${PROJECT_NAME} ${SOURCES})

#Includes
target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")


target_include_directories(${PROJECT_NAME} PRIVATE "./include" "/usr/include/stb")