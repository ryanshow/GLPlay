cmake_minimum_required(VERSION 3.6)
project(GLPlay)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(OPTION_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(OPTION_BUILD_GPU_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/glbinding)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/glfw)

set(SOURCE_FILES main.cpp)
add_executable(GLPlay ${SOURCE_FILES})

target_compile_definitions(GLPlay PRIVATE GLFW_INCLUDE_NONE=1)
target_link_libraries(GLPlay glfw glbinding)