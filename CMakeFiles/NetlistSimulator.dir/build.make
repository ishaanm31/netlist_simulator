# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/harshraj/netlist_simulator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/harshraj/netlist_simulator

# Include any dependencies generated for this target.
include CMakeFiles/NetlistSimulator.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/NetlistSimulator.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/NetlistSimulator.dir/flags.make

CMakeFiles/NetlistSimulator.dir/main.cpp.o: CMakeFiles/NetlistSimulator.dir/flags.make
CMakeFiles/NetlistSimulator.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/harshraj/netlist_simulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/NetlistSimulator.dir/main.cpp.o"
	/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/NetlistSimulator.dir/main.cpp.o -c /home/harshraj/netlist_simulator/main.cpp

CMakeFiles/NetlistSimulator.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/NetlistSimulator.dir/main.cpp.i"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/harshraj/netlist_simulator/main.cpp > CMakeFiles/NetlistSimulator.dir/main.cpp.i

CMakeFiles/NetlistSimulator.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/NetlistSimulator.dir/main.cpp.s"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/harshraj/netlist_simulator/main.cpp -o CMakeFiles/NetlistSimulator.dir/main.cpp.s

CMakeFiles/NetlistSimulator.dir/netlist_parser.cpp.o: CMakeFiles/NetlistSimulator.dir/flags.make
CMakeFiles/NetlistSimulator.dir/netlist_parser.cpp.o: netlist_parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/harshraj/netlist_simulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/NetlistSimulator.dir/netlist_parser.cpp.o"
	/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/NetlistSimulator.dir/netlist_parser.cpp.o -c /home/harshraj/netlist_simulator/netlist_parser.cpp

CMakeFiles/NetlistSimulator.dir/netlist_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/NetlistSimulator.dir/netlist_parser.cpp.i"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/harshraj/netlist_simulator/netlist_parser.cpp > CMakeFiles/NetlistSimulator.dir/netlist_parser.cpp.i

CMakeFiles/NetlistSimulator.dir/netlist_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/NetlistSimulator.dir/netlist_parser.cpp.s"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/harshraj/netlist_simulator/netlist_parser.cpp -o CMakeFiles/NetlistSimulator.dir/netlist_parser.cpp.s

CMakeFiles/NetlistSimulator.dir/netlist.cpp.o: CMakeFiles/NetlistSimulator.dir/flags.make
CMakeFiles/NetlistSimulator.dir/netlist.cpp.o: netlist.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/harshraj/netlist_simulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/NetlistSimulator.dir/netlist.cpp.o"
	/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/NetlistSimulator.dir/netlist.cpp.o -c /home/harshraj/netlist_simulator/netlist.cpp

CMakeFiles/NetlistSimulator.dir/netlist.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/NetlistSimulator.dir/netlist.cpp.i"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/harshraj/netlist_simulator/netlist.cpp > CMakeFiles/NetlistSimulator.dir/netlist.cpp.i

CMakeFiles/NetlistSimulator.dir/netlist.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/NetlistSimulator.dir/netlist.cpp.s"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/harshraj/netlist_simulator/netlist.cpp -o CMakeFiles/NetlistSimulator.dir/netlist.cpp.s

# Object files for target NetlistSimulator
NetlistSimulator_OBJECTS = \
"CMakeFiles/NetlistSimulator.dir/main.cpp.o" \
"CMakeFiles/NetlistSimulator.dir/netlist_parser.cpp.o" \
"CMakeFiles/NetlistSimulator.dir/netlist.cpp.o"

# External object files for target NetlistSimulator
NetlistSimulator_EXTERNAL_OBJECTS =

NetlistSimulator: CMakeFiles/NetlistSimulator.dir/main.cpp.o
NetlistSimulator: CMakeFiles/NetlistSimulator.dir/netlist_parser.cpp.o
NetlistSimulator: CMakeFiles/NetlistSimulator.dir/netlist.cpp.o
NetlistSimulator: CMakeFiles/NetlistSimulator.dir/build.make
NetlistSimulator: CMakeFiles/NetlistSimulator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/harshraj/netlist_simulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable NetlistSimulator"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/NetlistSimulator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/NetlistSimulator.dir/build: NetlistSimulator

.PHONY : CMakeFiles/NetlistSimulator.dir/build

CMakeFiles/NetlistSimulator.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/NetlistSimulator.dir/cmake_clean.cmake
.PHONY : CMakeFiles/NetlistSimulator.dir/clean

CMakeFiles/NetlistSimulator.dir/depend:
	cd /home/harshraj/netlist_simulator && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/harshraj/netlist_simulator /home/harshraj/netlist_simulator /home/harshraj/netlist_simulator /home/harshraj/netlist_simulator /home/harshraj/netlist_simulator/CMakeFiles/NetlistSimulator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/NetlistSimulator.dir/depend

