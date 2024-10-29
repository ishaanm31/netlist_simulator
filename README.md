# Sequential ATPG

- Ishaan Manhar, 210070033
- Harshraj Chaudhari, 210040060
- Atharva Kulkarni, 210070047
- Avaneesh Sai, 210070015

## Overview

This sequential ATPG comprises of the Netlist Simulator previously built in assignment 1 and uses PODEM algorithm to generate test vectors efficiently for the unrolled circuit. It has the following components:

- A parser to read netlist files - DFF functionality added.
- A simulator to evaluate the behavior of logic gates.
- A mechanism to manage wire connections and signals.
- Automated calculation of sequential depth, unrolling of the circuit across the flip-flops.
- Single-stuck-at-fault assignments are done across all timeframes but PODEM is applied one-by-one to each (except TF = -1).
- The faults whose test-vectors are common are grouped and finally printed along with the test-vectors.

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
    Use the bash script to generate the necessary build files:
    ```sh
    bash run.sh
   ```
   This builds and executes the program.
3. **Running the program without building**
Make sure you have completed step 2 atleast once.
    ```sh
    ./seq_atpg
    ```
## Examples
An example netlist is given "simple_netlist.txt".

A sample usage is given here

```sh
$ ./seq_atpg 
Assignment 2 of VLSI CAD 2024! 
Sequential Automatic Test Vector Generator using PODEM. 
Give filepath for the netlist (.txt)
simple_circuit.txt
Parsing the netlist
Module Name: simple_circuit
End of Netlist. Parsing Complete
Printing parsed data
Inputs: A B C 
Outputs: Z 
Wires: w1 w2 w3 
Gates:
Gate Type: AND2, Output: w1, Inputs: A, B
Gate Type: OR2, Output: w3, Inputs: w2, C
Gate Type: INV, Output: Z, Inputs: w4
FF:
DFF, Outputs: w2, Inputs: w1
DFF, Outputs: w4, Inputs: w3
flip-flops in the netlist
Sequential Depth: 2
Inputs: A_0 B_0 C_0 A_1 B_1 C_1 A_2 B_2 C_2 
Outputs: Z_0 Z_1 Z_2 
Wires: w1_0 w2_0 w3_0 w4_0 w1_1 w3_1 w1_2 w3_2 
Gates: 
w1_0 AND2 A_0 B_0 
w3_0 OR2 w2_0 C_0 
Z_0 INV w4_0 
w1_1 AND2 A_1 B_1 
w3_1 OR2 w1_0 C_1 
Z_1 INV w3_0 
w1_2 AND2 A_2 B_2 
w3_2 OR2 w1_1 C_2 
Z_2 INV w3_1 
FF: Faults: GID:0ip:0 | SA:0 GID:0ip:1 | SA:0 GID:0op | SA:0 GID:1ip:0 | SA:0 GID:1op | SA:0 GID:2ip:0 | SA:0 GID:2op | SA:1 PI:A | SA:0 PI:B | SA:0 PO:Z | SA:1 
Test Vector:
  A_0: 1
  A_1: -1
  A_2: -1
  B_0: 1
  B_1: -1
  B_2: -1
  C_0: 1
  C_1: 0
  C_2: -1
------------------
Faults: GID:0ip:0 | SA:1 GID:0op | SA:1 GID:1ip:0 | SA:1 GID:1ip:1 | SA:1 GID:1op | SA:1 GID:2ip:0 | SA:1 GID:2op | SA:0 PI:A | SA:1 PI:C | SA:1 PO:Z | SA:0 
Test Vector:
  A_0: 0
  A_1: -1
  A_2: -1
  B_0: 1
  B_1: -1
  B_2: -1
  C_0: -1
  C_1: 0
  C_2: -1
------------------
Faults: GID:0ip:1 | SA:1 PI:B | SA:1 
Test Vector:
  A_0: 1
  A_1: -1
  A_2: -1
  B_0: 0
  B_1: -1
  B_2: -1
  C_0: -1
  C_1: 0
  C_2: -1
------------------
Faults: GID:1ip:1 | SA:0 PI:C | SA:0 
Test Vector:
  A_0: 0
  A_1: -1
  A_2: -1
  B_0: -1
  B_1: -1
  B_2: -1
  C_0: -1
  C_1: 1
  C_2: -1
------------------
```