cmake_minimum_required(VERSION 3.10)
project(tnps)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_DEBUG "-g")


# Collect all source files
file(GLOB_RECURSE SOURCES
    source/*.cc
)

add_executable(tnps ${SOURCES})
