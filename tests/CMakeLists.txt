# Download and link GTest via FetchContent (modern CMake way)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add test sources
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS *.cpp)
add_executable(runTests
    ${TEST_SOURCES}
)

target_link_libraries(runTests
    GTest::gtest_main
    tanks_game_lib
)

# Register the tests
include(GoogleTest)
gtest_discover_tests(runTests)
