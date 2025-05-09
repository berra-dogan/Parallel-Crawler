# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Files
TARGET = main
SRC = main.cpp PageProcessing/PageProcessing.cpp
OBJ = $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create the final executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean object files and the executable
clean:
	rm -f $(OBJ) $(TARGET)
