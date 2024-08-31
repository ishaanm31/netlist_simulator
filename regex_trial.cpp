#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <sstream>

struct Gate {
    std::string type;
    std::string instance_name;
    std::string output;
    std::vector<std::string> inputs;
};

int main() {
    // Netlist as a vector of strings
    std::vector<std::string> verilog_code = {
        "module simple_circuit (",
        "output out1,",
        "input in1, in2, in3",
        ");",
        "wire w1, w2;",
        "// Gates",
        "and u1 (w1,in1,in2) ;",
        "or  u2 (out1,w1,in3) ;",
        "endmodule"
    };

    std::string module_name;
    std::vector<std::pair<std::string, std::string>> ports;
    std::vector<std::string> wires;
    std::vector<Gate> gates;

    std::regex module_regex(R"(\bmodule\s+(\w+)\s*\()");
    std::regex port_regex(R"((output|input)\s+([\w\s,]+);?)");
    std::regex wire_regex(R"(wire\s+([\w\s,]+);)");
    std::regex gate_regex(R"((\w+)\s+(\w+)\s*\(([\w\s,]+)\)\s*;)");

    for (const std::string& line : verilog_code) {
        std::smatch match;

        // Match module name
        if (std::regex_search(line, match, module_regex)) {
            module_name = match[1];
        }

        // Match ports
        else if (std::regex_search(line, match, port_regex)) {
            ports.push_back({match[1], match[2]});
        }

        // Match wires
        else if (std::regex_search(line, match, wire_regex)) {
            std::string wire_list = match[1];
            std::istringstream wire_stream(wire_list);
            std::string wire;
            while (std::getline(wire_stream, wire, ',')) {
                wires.push_back(wire);
            }
        }

        // Match gates
        else if (std::regex_search(line, match, gate_regex)) {
            std::string gate_type = match[1];
            std::string instance_name = match[2];
            std::string connections = match[3];

            // Split connections
            std::vector<std::string> connections_list;
            std::istringstream connection_stream(connections);
            std::string connection;
            while (std::getline(connection_stream, connection, ',')) {
                connections_list.push_back(connection);
            }

            // Assign output and input connections
            std::string output = connections_list[0];
            std::vector<std::string> inputs(connections_list.begin() + 1, connections_list.end());

            gates.push_back({gate_type, instance_name, output, inputs});
        }
    }

    // Output the results
    std::cout << "Module Name: " << module_name << std::endl;

    std::cout << "Ports:" << std::endl;
    for (const auto& port : ports) {
        std::cout << "  " << port.first << ": " << port.second << std::endl;
    }

    std::cout << "Wires: ";
    for (size_t i = 0; i < wires.size(); ++i) {
        std::cout << wires[i];
        if (i < wires.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;

    std::cout << "Gate Instances:" << std::endl;
    for (const auto& gate : gates) {
        std::cout << "  " << gate.type << " " << gate.instance_name << ":" << std::endl;
        std::cout << "    Output: " << gate.output << std::endl;
        std::cout << "    Inputs: ";
        for (size_t i = 0; i < gate.inputs.size(); ++i) {
            std::cout << gate.inputs[i];
            if (i < gate.inputs.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }

    return 0;
}
