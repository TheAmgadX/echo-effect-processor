cmake_minimum_required(VERSION 3.13)
project(SoundProcessingOpt VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the output directory for executables to "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Common source files (without main or benchmark)
set(COMMON_SOURCES
    src/AudioFile.cpp
    src/echo_processor.cpp
    src/Visuals.cpp
    src/echo_naive.cpp
    src/echo_simd_opt.cpp
)

# main executable
add_executable(echo_processor
    src/main.cpp
    ${COMMON_SOURCES}
)
# Disable optimizations
set(CMAKE_CXX_FLAGS_RELEASE "")