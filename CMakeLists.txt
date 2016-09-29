cmake_minimum_required(VERSION 3.6)
project(GLPlay)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(OPTION_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(OPTION_BUILD_GPU_TESTS OFF CACHE BOOL "" FORCE)
#set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/glbinding)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
SET(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
add_subdirectory(extern/glfw)

add_subdirectory(extern/fmt)

set(SOURCE_FILES src/main.cpp)
add_executable(GLPlay ${SOURCE_FILES})

target_compile_definitions(GLPlay PUBLIC GLFW_INCLUDE_NONE=1)
target_link_libraries(GLPlay PRIVATE glfw glbinding fmt)
target_include_directories(GLPlay PUBLIC extern/glm extern/stb extern/fmt generated/include)

set_target_properties(glbinding PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set_target_properties(glfw PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set_target_properties(fmt PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_custom_command(
        TARGET GLPlay
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:GLPlay>/assets
)

include(cmake/bin2h.cmake)

set(create_file YES)
file(GLOB files "resources/shaders/*.glsl")
list(REMOVE_DUPLICATES files)
foreach(file ${files})
    get_filename_component(file_name ${file} NAME_WE)
    if (create_file)
        bin2h(
                SOURCE_FILE ${file}
                HEADER_FILE generated/include/shaders.h
                VARIABLE_NAME ${file_name}
                NULL_TERMINATE
        )
    else()
        bin2h(
                SOURCE_FILE ${file}
                HEADER_FILE generated/include/shaders.h
                VARIABLE_NAME ${file_name}
                APPEND
                NULL_TERMINATE
        )
    endif()
    set(create_file NO)
endforeach()
