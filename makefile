# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Files
TARGET = main
SRC = main.cpp $(wildcard src/*.cpp)
OBJ = $(patsubst %.cpp, build/%.o, $(SRC))

# Default target
all: $(TARGET)

# Link object files to create the final executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files (ensure build/ exists)
build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean object files and the executable
clean:
	rm -rf build $(TARGET)
