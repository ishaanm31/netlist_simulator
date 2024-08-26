#include <iostream>
#include <fstream>
#include <sstream>
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
    void parse(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Unable to open file: " << filename << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            line = trim(line);
            if (line.empty() || line[0] == '#') {
                continue; // Skip empty lines and comments
            }
            processLine(line);
        }

        file.close();
    }

    // Function to display the parsed netlist
    void printParsedData() const {
        std::cout << "Inputs: ";
        for (const auto& input : netlist.inputs) {
            std::cout << input << " ";
        }
        std::cout << std::endl;

        std::cout << "Outputs: ";
        for (const auto& output : netlist.outputs) {
            std::cout << output << " ";
        }
        std::cout << std::endl;

        std::cout << "Wires: ";
        for (const auto& wire : netlist.wires) {
            std::cout << wire << " ";
        }
        std::cout << std::endl;

        std::cout << "Gates:" << std::endl;
        for (const auto& gate : netlist.gates) {
            std::cout << "Gate Type: " << gate.type << ", Output: " << gate.output << ", Inputs: ";
            for (size_t i = 0; i < gate.inputs.size(); ++i) {
                std::cout << gate.inputs[i];
                if (i != gate.inputs.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }
    }

private:
    Netlist netlist;

    // Function to process each line of the netlist
    void processLine(const std::string& line) {
        std::regex inputRegex(R"(^input\s+([\w\s,]+);$)");
        std::regex outputRegex(R"(^output\s+([\w\s,]+);$)");
        std::regex wireRegex(R"(^wire\s+([\w\s,]+);$)");
        std::regex gateRegex(R"((\w+)\s+(\w+)\(([\w\s,]+)\);$)");

        std::smatch match;

        if (std::regex_search(line, match, inputRegex)) {
            parseSignals(match[1].str(), netlist.inputs);
        } else if (std::regex_search(line, match, outputRegex)) {
            parseSignals(match[1].str(), netlist.outputs);
        } else if (std::regex_search(line, match, wireRegex)) {
            parseSignals(match[1].str(), netlist.wires);
        } else if (std::regex_search(line, match, gateRegex)) {
            Gate gate;
            gate.output = match[1].str();
            gate.type = match[2].str();
            std::string inputs = match[3].str();
            std::stringstream ss(inputs);
            std::string input;
            while (std::getline(ss, input, ',')) {
                gate.inputs.push_back(trim(input));
            }
            netlist.gates.push_back(gate);
        } else {
            std::cerr << "Unrecognized line format: " << line << std::endl;
        }
    }

    // Helper function to parse signals (inputs, outputs, wires)
    void parseSignals(const std::string& signalString, std::vector<std::string>& signalList) {
        std::stringstream ss(signalString);
        std::string signal;
        while (std::getline(ss, signal, ',')) {
            signalList.push_back(trim(signal));
        }
    }

    // Helper function to trim whitespace from a string
    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(' ');
        size_t last = str.find_last_not_of(' ');
        if (first == std::string::npos || last == std::string::npos) {
            return "";
        }
        return str.substr(first, (last - first + 1));
    }
};

int main() {
    netlist_parser parser;
    parser.parse("/home/ishaan/Study/VLSI CAD/netlist.txt");
    parser.printParsedData();

    return 0;
}
