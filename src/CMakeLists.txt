# Add all source files
file(GLOB_RECURSE TANKS_GAME_LIB_SOURCES CONFIGURE_DEPENDS *.cpp)

file(GLOB_RECURSE TANKS_GAME_LIB_HEADERS CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/*.h
    ${PROJECT_SOURCE_DIR}/src/*.hpp
)


add_library(tanks_game_lib
    ${TANKS_GAME_LIB_SOURCES}
)
message(STATUS "Sources: ${TANKS_GAME_LIB_SOURCES}")
message(STATUS "Headers: ${TANKS_GAME_LIB_HEADERS}")


target_include_directories(tanks_game_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/common
)