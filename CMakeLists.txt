cmake_minimum_required(VERSION 3.6)

project(GLPlay)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_subdirectory(extern/fmt)
set_target_properties(fmt PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

set(SOURCE_FILES src/main.cc src/viewport.h src/viewport.cc src/window.h src/window.cc src/utils.h src/bitmap_font.h src/bitmap_font.cc src/renderable.h src/renderable.cpp src/ui_overlay.h src/ui_overlay.cpp src/event_source.h src/vertex.h src/ortho_viewport.h src/ortho_viewport.cpp src/perspective_viewport.h src/perspective_viewport.cpp src/scene.h src/entity.h)
add_executable(GLPlay ${SOURCE_FILES})

if (EMSCRIPTEN)
  target_compile_definitions(GLPlay PUBLIC GL_GLEXT_PROTOTYPES=1)
  set(CMAKE_EXE_LINKER_FLAGS "-s USE_GLFW=3")
  set(CMAKE_EXECUTABLE_SUFFIX ".html")

  target_link_libraries(GLPlay PRIVATE
    fmt
  )
else()
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
  add_subdirectory(extern/glfw)
  target_compile_definitions(GLPlay PUBLIC GLFW_INCLUDE_NONE=1)
  set_target_properties(glfw PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

  set(OPTION_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(OPTION_BUILD_GPU_TESTS OFF CACHE BOOL "" FORCE)
  set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
  add_subdirectory(extern/glbinding)
  set_target_properties(glbinding PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

  target_link_libraries(GLPlay PRIVATE
    glfw
    glbinding
    fmt
  )
endif()

target_include_directories(GLPlay PUBLIC extern/glm extern/stb extern/fmt ${CMAKE_BINARY_DIR}/generated/include)

add_custom_command(
        TARGET GLPlay
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:GLPlay>/assets
)

include(cmake/bin2h.cmake)

file(GLOB resource_types RELATIVE ${CMAKE_SOURCE_DIR}/resources "${CMAKE_SOURCE_DIR}/resources/*")

foreach(resource_type ${resource_types})
    file(GLOB resource_files RELATIVE ${CMAKE_SOURCE_DIR}/resources/${resource_type} "${CMAKE_SOURCE_DIR}/resources/${resource_type}/*")
    file(WRITE ${CMAKE_BINARY_DIR}/generated/include/resources/${resource_type}.h "// This file is generated automatically\n")
    foreach(resource_file ${resource_files})
        get_filename_component(resource ${resource_file} NAME_WE)
        message("RESOURCES: Appending ${resource} to ${resource_type}.h")
        bin2h(
            SOURCE_FILE ${CMAKE_SOURCE_DIR}/resources/${resource_type}/${resource_file}
            HEADER_FILE ${CMAKE_BINARY_DIR}/generated/include/resources/${resource_type}.h
            VARIABLE_NAME ${resource}
            APPEND
            NULL_TERMINATE
        )
    endforeach()
endforeach()
