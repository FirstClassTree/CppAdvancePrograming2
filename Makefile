# === CONFIG ===
CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -g -MMD -MP -Isrc -Isrc/entities -Isrc/game_env -Isrc/game_algo -Isrc/utils
LDFLAGS := 

# List all source files
SRCS := \
	$(wildcard src/*.cpp) \
	$(wildcard src/game_algo/*.cpp) \
	$(wildcard src/game_env/*.cpp) \
	$(wildcard src/utils/*.cpp) \
	$(wildcard src/entities/*.cpp) \

OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

TARGET := tanks_game

# === RULES ===

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /q $(subst /,\,$(OBJS) $(DEPS)) tanks_game.exe 2>nul || exit 0

run: all
	./$(TARGET) $(ARGS)

.PHONY: all clean run