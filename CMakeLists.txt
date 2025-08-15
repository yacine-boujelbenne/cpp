cmake_minimum_required(VERSION 3.14)
project(CanDemo)

# Download and build Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.14.0
)
FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wextra")

# Include headers directory
include_directories(headers)

# Create library from source files
add_library(CanLib STATIC
    src/Can.cpp
    src/CanBus.cpp
    src/CanTp.cpp
    src/Receiver.cpp
    src/Sender.cpp
)

# Main executable
add_executable(Main src/Main.cpp)
target_link_libraries(Main CanLib)

# Unit test configuration
enable_testing()

# Create unit test executable
add_executable(UnitTests
    tests/test_Can.cpp
    tests/test_CanBus.cpp
    tests/test_CanTp.cpp
    tests/test_Receiver.cpp
    tests/test_Sender.cpp
)

# Link test executable with library and Google Test
target_link_libraries(UnitTests
    CanLib
    GTest::gtest_main
    pthread
)

# Add Google Test discovery
include(GoogleTest)
gtest_discover_tests(UnitTests
    EXTRA_ARGS --gtest_output=xml:test_results.xml
)