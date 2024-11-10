# Makefile for the XML GUI Testing Project

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17

# SDL2 linking
SDL2_LIBS = -lSDL2

# Include directories for header files
INCLUDES = -I. -Igui -Iparse

# Source files and object files
SRCS = tests/test_gui_file.cpp gui/GUIFile.cpp parse/parse.cpp
OBJS = tests/test_gui_file.o gui/GUIFile.o parse/parse.o

# Executable name
EXEC = test

# Default target
all: $(EXEC)

# Rule to build the executable from object files
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC) $(SDL2_LIBS)

# Compile individual source files into object files
tests/test_gui_file.o: tests/test_gui_file.cpp gui/GUIFile.hpp parse/parse.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c tests/test_gui_file.cpp -o tests/test_gui_file.o

gui/GUIFile.o: gui/GUIFile.cpp gui/GUIFile.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c gui/GUIFile.cpp -o gui/GUIFile.o

parse/parse.o: parse/parse.cpp parse/parse.hpp gui/GUIFile.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c parse/parse.cpp -o parse/parse.o

# Clean up the build
clean:
	rm -f $(EXEC) $(OBJS) output.xml