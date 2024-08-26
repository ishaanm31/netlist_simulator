#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct Gate {
    string type;
    string instance_name;
    string output;
    vector<string> inputs;
};

class VerilogParser {
private:
    vector<Gate> gates;
    map<string, string> signals; // Map signal name to its type (input, output, wire)

public:
    void parseFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Unable to open file: " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string word;
            ss >> word;

            if (word == "input" || word == "output" || word == "wire") {
                parseSignalDeclaration(word, line);
            } else {
                parseGate(line);
            }
        }

        file.close();
    }

    void parseSignalDeclaration(const string& type, const string& line) {
        stringstream ss(line);
        string signal_type, signal_name;

        ss >> signal_type; // Skip the first word (input/output/wire)
        while (ss >> signal_name) {
            // Remove commas and semicolons
            if (signal_name.back() == ',' || signal_name.back() == ';') {
                signal_name.pop_back();
            }
            signals[signal_name] = signal_type;
        }
    }

void parseGate(const string& line) {
    stringstream ss(line);
    Gate gate;
    string word;

    ss >> gate.type; // Gate type (e.g., and, or, etc.)
    ss >> gate.instance_name; // Instance name (e.g., u1, u2, etc.)

    string connections;
    ss >> connections; // Entire connection part (e.g., (w1, in1, in2))
    cout<<gate.type<<" && "<<gate.instance_name<<" && "<<connections<<endl;
    if (connections.size() >= 2 && connections.front() == '(' && connections.back() == ')') {
        // Remove parentheses
        connections = connections.substr(1, connections.size() - 2);
    } else {
        cerr << "Error: Invalid gate connection format in line: " << line << endl;
        return;
    }

    // Split by commas
    stringstream connection_stream(connections);
    string connection;
    vector<string> pins;
    while (getline(connection_stream, connection, ',')) {
        pins.push_back(connection);
    }

    if (!pins.empty()) {
        gate.output = pins[0];
        gate.inputs.assign(pins.begin() + 1, pins.end());
    }

    gates.push_back(gate);
}


    void display() {
        for (const auto& signal : signals) {
            cout << "Signal: " << signal.first << ", Type: " << signal.second << endl;
        }

        for (const auto& gate : gates) {
            cout << "Gate Type: " << gate.type << ", Instance: " << gate.instance_name << endl;
            cout << "  Output: " << gate.output << endl;
            cout << "  Inputs: ";
            for (const auto& input : gate.inputs) {
                cout << input << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    VerilogParser parser;
    parser.parseFile("/home/ishaan/Study/VLSI CAD/simple_circuit.txt"); // Replace with your actual netlist filename
    parser.display();

    return 0;
}
