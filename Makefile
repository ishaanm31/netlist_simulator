# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

# Executable name
TARGET = netlist_simulator

# Source files
SRCS = main.cpp netlist.cpp netlist_parser.cpp node.cpp operation.cpp port.cpp wire.cpp

# Default rule to build the executable
all: $(TARGET)

# Rule to directly compile and link the source files into the executable
$(TARGET):
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Clean up the executable
clean:
	rm -f $(TARGET)
