#include "netlist_parser.hpp"
#include <fstream>
#include <sstream>

// Function to parse the netlist file
void netlist_parser::parse(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        exit(1);
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
void netlist_parser::printParsedData() const {
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

// Function to process each line of the netlist
void netlist_parser::processLine(const std::string& line) {

    std::regex moduleRegex(R"(module\s+(\w+)\s*\()");
    std::regex commentRegex(R"(//|\);)");
    std::regex portRegex(R"((output|input)\s+([\w\s,]+);?)");
    std::regex wireRegex(R"(wire\s+([\w\s,]+);)");
    std::regex gateRegex(R"((\w+)\s+(\w+)\s*\(([\w\s,]+)\)\s*;)");
    std::regex endmoduleRegex(R"(endmodule)");

    std::smatch match;

    if (std::regex_search(line, match, portRegex)) {
        if(match[1].str() == "input") {
            parseSignals(match[2].str(), netlist.inputs);
        } else if (match[1].str() == "output") {
            parseSignals(match[2].str(), netlist.outputs);
        }
    } else if (std::regex_search(line, match, wireRegex)) {
        parseSignals(match[1].str(), netlist.wires);
    } else if (std::regex_search(line, match, gateRegex)) {
        Gate gate;
        gate.type = match[1].str();
        std::string connections = match[3].str();
        std::istringstream connection_stream(connections);
        std::string connection;
        std::vector<std::string> connectionsList;
        while (std::getline(connection_stream, connection, ',')) {
            connectionsList.push_back(trim(connection));
        }
        gate.output = connectionsList[0];
        for (size_t i = 1; i < connectionsList.size(); ++i) {
            gate.inputs.push_back(connectionsList[i]);
        }
        netlist.gates.push_back(gate);

    } else if (std::regex_search(line, match, moduleRegex)) {
        std::cout << "Module Name: " << match[1].str() << std::endl;
    } else if(std::regex_search(line, match, commentRegex)) {
        // Skip comments
    } else if(std::regex_search(line, match, endmoduleRegex)) {
        std::cout << "End of Netlist. Parsing Complete" << std::endl;
    }
    else {
        std::cerr << "Unrecognized line format: " << line << std::endl;
    }
}

// Helper function to parse signals (inputs, outputs, wires)
void netlist_parser::parseSignals(const std::string& signalString, std::vector<std::string>& signalList) {
    std::stringstream ss(signalString);
    std::string wire;
    while (std::getline(ss, wire, ',')) {
        signalList.push_back(trim(wire));
    }
}

// Helper function to trim whitespace from a string
std::string netlist_parser::trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    if (first == std::string::npos || last == std::string::npos) {
        return "";
    }
    return str.substr(first, (last - first + 1));
}
