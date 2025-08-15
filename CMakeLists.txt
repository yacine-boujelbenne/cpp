cmake_minimum_required(VERSION 3.10)
project(CanDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wextra -Werror")

# Find GoogleTest
find_package(GTest REQUIRED)
include(GoogleTest)

# List all source files
file(GLOB SOURCES
    "Can.cpp"
    "CanBus.cpp"
    "CanTp.cpp"
    "Main.cpp"
    "Receiver.cpp"
    "Sender.cpp"
)

# Main executable
add_executable(Main ${SOURCES})

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
target_link_libraries(UnitTests GTest::GTest GTest::Main pthread)

# Add Google Test discovery
gtest_discover_tests(UnitTests
    EXTRA_ARGS --gtest_output=xml:test_results.xml
)