cmake_minimum_required(VERSION 3.6)

project(GLPlay)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES
    src/main.cc
    src/viewport.h
    src/viewport.cc
    src/window.h
    src/window.cc
    src/utils.h
    src/bitmap_font.h
    src/bitmap_font.cc
    src/renderable.h
    src/renderable.cpp
    src/ui_overlay.h
    src/ui_overlay.cpp
    src/event_source.h
    src/vertex.h
    src/ortho_viewport.h
    src/ortho_viewport.cpp
    src/perspective_viewport.h
    src/perspective_viewport.cpp
    src/scene.h
    src/entity.h
    src/shader.h
    src/resource.h
    src/shader.cpp
    src/raw_resource.h
    src/raw_resource.cpp
    src/shader_resource.h
    src/shader_resource.cpp
    src/mesh_resource.h
    src/mesh_resource.cpp
)

add_executable(GLPlay ${SOURCE_FILES})

add_subdirectory(extern/xxd)

add_subdirectory(extern/fmt)
set_target_properties(fmt PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")


set(PHYSFS_ARCHIVE_GRP OFF CACHE BOOL "" FORCE)
set(PHYSFS_ARCHIVE_WAD OFF CACHE BOOL "" FORCE)
set(PHYSFS_ARCHIVE_HOG OFF CACHE BOOL "" FORCE)
set(PHYSFS_ARCHIVE_MVL OFF CACHE BOOL "" FORCE)
set(PHYSFS_ARCHIVE_QPAK OFF CACHE BOOL "" FORCE)
set(PHYSFS_ARCHIVE_SLB OFF CACHE BOOL "" FORCE)
set(PHYSFS_HAVE_CDROM_SUPPORT OFF CACHE BOOL "" FORCE)
set(PHYSFS_BUILD_STATIC OFF CACHE BOOL "" FORCE)
set(PHYSFS_BUILD_SHARED ON CACHE BOOL "" FORCE)
set(PHYSFS_BUILD_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/physfs)
set_target_properties(physfs PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

list(APPEND TARGET_LIBRARIES
    fmt
    physfs
)

if (EMSCRIPTEN)
    target_compile_definitions(GLPlay PUBLIC GL_GLEXT_PROTOTYPES=1)
    set(CMAKE_EXE_LINKER_FLAGS "-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
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

    list(APPEND TARGET_LIBRARIES
        glfw
        glbinding
    )
endif()

target_link_libraries(GLPlay PRIVATE ${TARGET_LIBRARIES})

target_include_directories(GLPlay PUBLIC
    extern/glm
    extern/stb
    extern/fmt
    extern/physfs/src
    extern/json/src
    ${CMAKE_BINARY_DIR}/generated/include
)

add_custom_target(ZipResources
    COMMENT "=== Zipping resources ==="
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/generated/include
    COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${CMAKE_BINARY_DIR}/generated/resources.7z" --format=7zip "resources"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_custom_target(DumpZippedResources
    COMMENT "=== Dumping resources zip to hex ==="
    COMMAND xxd -i "generated/resources.7z" "generated/include/resource_data.h"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    DEPENDS xxd ZipResources
)

add_dependencies(GLPlay DumpZippedResources)

