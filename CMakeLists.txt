cmake_minimum_required(VERSION 3.14)
project(CanTpProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include headers
include_directories(headers)

# Collect source files
file(GLOB SRC_FILES src/*.cpp)

# Build main library
add_library(cantp_lib ${SRC_FILES})

# -------- GoogleTest ----------
add_subdirectory(googletest)   # assumes you cloned gtest here
enable_testing()

# -------- Tests ---------------
file(GLOB TEST_SRC tests/*.cpp)

add_executable(run_tests ${TEST_SRC})

target_link_libraries(run_tests
    cantp_lib
    gtest
    gtest_main
    pthread
)

add_test(NAME CanTpTests COMMAND run_tests)
