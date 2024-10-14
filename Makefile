# Makefile for the XML GUI Testing Project

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17

# SDL2 linking
SDL2_LIBS = -lSDL2

# Source files
SRCS = test_gui_file.cpp 
EXEC = test

# Default target
all: $(EXEC)

# Rule to build the executable
$(EXEC): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(EXEC) $(SDL2_LIBS)

# Clean up the build
clean:
	rm -f $(EXEC) output.xml