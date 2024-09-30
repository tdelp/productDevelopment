# Makefile for SDL2 project

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17

# SDL2 linking
SDL2_LIBS = -lSDL2

# Source files and executable name
SRCS = sdl.cpp
EXEC = sdl

# Default target
all: $(EXEC)

# Rule to build the executable
$(EXEC): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(EXEC) $(SDL2_LIBS)

# Clean up the build
clean:
	rm -f $(EXEC)
