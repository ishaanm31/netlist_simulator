cmake_minimum_required(VERSION 3.10)  # Specify the CMake version required

# Set the project name and version
project(NetlistParserProject VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the build type to Debug for more verbose output
set(CMAKE_BUILD_TYPE Debug)

# Add verbose compiler flags
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -v")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /VERBOSE")
endif()

# Add executable target
add_executable(NetlistSimulator
    main.cpp
    netlist_parser.cpp
    netlist.cpp
)

# Include directories for header files
target_include_directories(NetlistSimulator PRIVATE
    ${CMAKE_SOURCE_DIR}
)

# Optionally link libraries here if you have any
# target_link_libraries(NetlistSimulator PRIVATE some_library)
