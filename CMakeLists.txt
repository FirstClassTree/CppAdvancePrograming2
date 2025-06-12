cmake_minimum_required(VERSION 3.10)
project(TanksGame2)

# Set C++ version
# set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXHX_FLAGS} -fsanitize=address -g")



# Add source files
add_subdirectory(src)

# Enable testing
enable_testing()
add_subdirectory(tests)

add_executable(tanks_game src/main.cpp)
target_link_libraries(tanks_game PRIVATE tanks_game_lib ncurses)
target_include_directories(tanks_game PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(tanks_game_lib PRIVATE ${CMAKE_SOURCE_DIR}/include)
