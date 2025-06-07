# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lcurl -latomic

# Source and build folders
SRC_DIR = src
BUILD_DIR = build
MAIN_DIR = src/bin

# Source files
COMMON_SRC = $(wildcard $(SRC_DIR)/*.cpp)

# Convert src/Foo.cpp to build/Foo.o
COMMON_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(COMMON_SRC))

# Targets
TARGETS := $(BUILD_DIR)/main1 $(BUILD_DIR)/main2 $(BUILD_DIR)/game $(BUILD_DIR)/hashset_benchmarking

# Default target: build both
all: $(TARGETS)

# Rules for each target
$(BUILD_DIR)/main1: $(MAIN_DIR)/main1.cpp $(COMMON_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/main2: $(MAIN_DIR)/main2.cpp $(COMMON_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/benchmark: $(MAIN_DIR)/benchmark.cpp $(COMMON_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/game: $(MAIN_DIR)/game.cpp $(COMMON_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/hashset_benchmarking: $(MAIN_DIR)/hashset_benchmarking.cpp $(COMMON_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile src/Foo.cpp → build/Foo.o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR) $(TARGETS)
