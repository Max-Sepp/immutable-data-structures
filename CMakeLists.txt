cmake_minimum_required(VERSION 3.21)
project(immutable-data-structures)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Main executable
add_executable(main src/main.cpp ${APP_SOURCES})
target_include_directories(main PRIVATE src)
