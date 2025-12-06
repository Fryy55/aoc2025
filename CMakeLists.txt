cmake_minimum_required(VERSION 3.16)
project(AoC2025 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_program(SCCACHE_EXEC sccache)
if (SCCACHE_EXEC)
    message(STATUS "Found sccache at ${SCCACHE_EXEC}, enabling")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${SCCACHE_EXEC})
else()
    message(FATAL_ERROR "sccache not found")
endif()


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    src/
)

include("cmake/CPM.cmake")

set(AURORA_USE_NAMESPACE ON)
CPMAddPackage("gh:Fryy55/aurora@1.1.2")
target_link_libraries(${PROJECT_NAME} Aurora)