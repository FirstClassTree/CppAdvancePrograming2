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
# add_executable(output_printer_test tests/output_printer_test.cpp)
# target_link_libraries(output_printer_test
#     GTest::gtest_main
#     tanks_game_lib
# )


# Optional: Disable AddressSanitizer with cmake -DENABLE_ASAN=OFF
# option(ENABLE_ASAN "Enable AddressSanitizer" OFF)

# if (ENABLE_ASAN)
#     message(STATUS "AddressSanitizer ENABLED")

#     # Apply ASan flags with helpful defaults
#     target_compile_options(runTests PRIVATE
#         -fsanitize=address
#         -g
#         -O0
#         -fno-omit-frame-pointer  # for better stack traces
#     )
#     target_link_options(runTests PRIVATE
#         -fsanitize=address
#     )

#     # Optional: Set runtime environment variables for verbose ASan
#     set(ENV{ASAN_OPTIONS} "verbosity=1:abort_on_error=1")
# endif()


# Register the tests
include(GoogleTest)
gtest_discover_tests(runTests)
