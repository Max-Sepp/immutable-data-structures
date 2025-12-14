cmake_minimum_required(VERSION 3.21)
project(immutable-data-structures)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Main executable
add_executable(main src/main.cpp ${APP_SOURCES})
target_include_directories(main PRIVATE src)

# --- Testing ---
# Only include tests if the TESTS option is ON (default: OFF)
option(BUILD_TESTS "Build the tests" OFF)

# Automatically enable tests for Debug builds unless the user explicitly disabled them.
if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT BUILD_TESTS)
    set(BUILD_TESTS ON CACHE BOOL "Build the tests" FORCE)
endif ()

if (BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    # Add the linkedlist tests executable
    add_executable(linkedlist_tests
            tests/LinkedListTests.cpp
    )
    target_link_libraries(linkedlist_tests PRIVATE GTest::gtest_main)
    target_include_directories(linkedlist_tests PRIVATE src)

    add_executable(deque_tests
            tests/DequeTests.cpp
    )
    target_link_libraries(deque_tests PRIVATE GTest::gtest_main)
    target_include_directories(deque_tests PRIVATE src)

    include(GoogleTest)
    gtest_discover_tests(linkedlist_tests)
endif ()
