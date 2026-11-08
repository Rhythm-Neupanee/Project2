# Compiler options
CXX = g++
CXXFLAGS = -std=c++11 -Iinclude

# Build the executable only (Standard for UNT CSE CELL grading)
all:
	$(CXX) $(CXXFLAGS) scr/*.cpp -o app

# Optional target to build and run manually
run: all
	./app

# Remove the compiled executable
clean:
	rm -f app

.PHONY: all run clean
