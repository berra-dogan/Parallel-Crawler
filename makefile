# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lcurl

# Source and build folders
SRC_DIR = src
BUILD_DIR = build

# Common source files
COMMON_SRC = $(wildcard $(SRC_DIR)/*.cpp)
COMMON_OBJ = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(COMMON_SRC))

# Targets
TARGETS = main game

# Default target: build both
all: $(TARGETS)

# Rules for each target
main: main.cpp $(COMMON_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

game: game.cpp $(COMMON_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile all object files under src/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR) $(TARGETS)
