#include <bits/stdc++.h>
#include "parser.hpp"
using namespace std;
// Written by Ishaan, Harshraj, Atharva and Avaneesh

int main(int argc, char *argv[]) {
    cout<<"Assignment 1 of VLSI CAD 2024!"<<endl;
    if (argc != 2) {
        cout<<"Give only one argument as the file directory of text file containing the netlist"<<endl;
        exit(1);
    }
    char* filepath = argv[1]; // File path for netlist
    cout<<"Parsing the netlist"<<endl;

    VerilogParser parser;
    parser.parseFile("simple_circuit.v"); // Replace with your actual netlist filename
    parser.display();

    return 0;

}