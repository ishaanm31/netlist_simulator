#ifndef netlist_parser_H
#define netlist_parser_H

#include <iostream>
#include <vector>
#include <string>
#include <regex>

// Structure to store information about a gate
struct Gate {
    std::string output;
    std::string type;
    std::vector<std::string> inputs;
};

// Structure to store the entire netlist
struct Netlist {
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    std::vector<std::string> wires;
    std::vector<Gate> gates;
};

class netlist_parser {
public:
    // Function to parse the netlist file
    void parse(const std::string& filename);

    // Function to display the parsed netlist
    void printParsedData() const;

private:
    Netlist netlist;

    // Function to process each line of the netlist
    void processLine(const std::string& line);

    // Helper function to parse signals (inputs, outputs, wires)
    void parseSignals(const std::string& signalString, std::vector<std::string>& signalList);

    // Helper function to trim whitespace from a string
    std::string trim(const std::string& str);
};

#endif // netlist_parser_H
