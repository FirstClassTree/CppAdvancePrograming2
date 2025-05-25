# Add all source files
file(GLOB_RECURSE TANKS_GAME_LIB_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(tanks_game_lib
    ${TANKS_GAME_LIB_SOURCES}
)
message(STATUS "Sources: ${TANKS_GAME_LIB_SOURCES}")

target_include_directories(tanks_game_lib PUBLIC ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/common)