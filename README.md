# Netlist Simulator

- Ishaan Manhar, 210070033
- Harshraj Chaudhari, 210040060
- Atharva Kulkarni, 210070047
- Avaanesh Sai, 210070015

## Overview

The Netlist Simulator is a powerful tool designed for parsing and simulating digital circuits based on a netlist description. It consists of several core components:

- A parser to read and understand netlist files.
- A simulator to evaluate the behavior of logic gates.
- A mechanism to manage wire connections and signals.

This project is ideal for those looking to analyze digital circuits or verify logical correctness based on a netlist description.

## Prerequisites

To build and run the Netlist Simulator, ensure you have the following installed:

- **CMake** (version 3.10 or higher)
- **C++17 compatible compiler** (e.g., GCC, Clang, or MSVC)

## Building the Project

Follow these steps to build the Netlist Simulator:

1. **Clone the Repository**

   Begin by cloning the repository to your local machine:

   ```sh
   git clone https://github.com/yourusername/netlist-simulator.git
   cd netlist-simulator
   ```
2. **Building the executable**
    Create a build directory and use CMake to generate the necessary build files:
    ```sh
    cmake .
    make
   ```
3. **Running the simulator**
    ```sh
    ./NetlistSimulator
    ```
## Examples
An example netlist is given "netlist.txt" use it for testing
A sample usage is given here

```sh
$ ./NetlistSimulator 
Assignment 1 of VLSI CAD 2024! 
Give filepath for the netlist.txt
/home/ishaan/Study/VLSI_CAD/netlist.txt
Parsing the netlist
For exiting the loop enter Q
Give input vector in the given format: for input A=0 B=1 C=0 write 010 in the command line
Inputs for our netlist are:
 A, B, C, 
111
Y --> 0
For exiting the loop enter Q
Give input vector in the given format: for input A=0 B=1 C=0 write 010 in the command line
Inputs for our netlist are:
 A, B, C, 
110
Y --> 0
For exiting the loop enter Q
Give input vector in the given format: for input A=0 B=1 C=0 write 010 in the command line
Inputs for our netlist are:
 A, B, C, 
100
Y --> 1
For exiting the loop enter Q
Give input vector in the given format: for input A=0 B=1 C=0 write 010 in the command line
Inputs for our netlist are:
 A, B, C, 
q
```