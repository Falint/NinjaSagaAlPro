cmake_minimum_required(VERSION 3.20)
project(NinjaSagaAlPro VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==================================
# Output Directory
# ==================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ==================================
# Raylib via FetchContent
# ==================================
include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        6.0
    GIT_SHALLOW    TRUE
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES   OFF CACHE BOOL "" FORCE)
set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(raylib)

# ==================================
# Source Files
# ==================================

file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    src/*.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})

# ==================================
# Include Path
# ==================================
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# ==================================
# Link Raylib
# ==================================
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# ==================================
# Platform Settings
# ==================================
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm)
endif()

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
    "-framework IOKit"
    "-framework Cocoa"
    "-framework OpenGL"
    )
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        m dl pthread)
endif()

# ==================================
# Copy Assets ke build/bin
# ==================================
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
        COMMENT "Copying assets to build directory..."
)
# ==================================
# Compiler Warnings
# ==================================
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

message(STATUS "=== Ninja Saga Build Config ===")
message(STATUS "Platform  : ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler  : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Output    : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
