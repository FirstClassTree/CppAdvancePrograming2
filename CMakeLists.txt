cmake_minimum_required(VERSION 3.10)
project(TanksGame2)

# Set C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# AddressSanitizer option
option(ENABLE_ASAN "Enable AddressSanitizer" ON)

if(ENABLE_ASAN)
  message(STATUS "AddressSanitizer enabled")
  set(ASAN_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ASAN_FLAGS}")
  set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ${ASAN_FLAGS}")
endif()

# Add source files
add_subdirectory(src)

# Enable testing
enable_testing()
add_subdirectory(tests)

add_executable(tanks_game src/main.cpp)
target_link_libraries(tanks_game PRIVATE tanks_game_lib ncurses)
