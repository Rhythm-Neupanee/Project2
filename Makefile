
# Compiler options
CXX = g++
CXXFLAGS = -std=c++11 -Iinclude

# Build the program
all:
	g++ $(CXXFLAGS) scr/*.cpp -o app
	.\app.exe

# Remove the program
clean:
	rm -f app

.PHONY: all clean