cmake_minimum_required(VERSION 3.10)
project(TanksGame2)

# Set C++ version
set(CMAKE_CXX_STANDARD 20)

# Add source files
add_subdirectory(src)

# Enable testing
enable_testing()
add_subdirectory(tests)

add_executable(tanks_game src/main.cpp)