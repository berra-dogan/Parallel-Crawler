# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -O2

# Output binary
TARGET = extract_links_gumbo

# Source files
SRCS = extract_links_gumbo.cpp

# Libraries
LIBS = -lcurl -lgumbo

# Default target
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Clean build artifacts
clean:
	rm -f $(TARGET)
