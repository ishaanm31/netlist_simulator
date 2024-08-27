#include "netlist.hpp"
#include "wire.hpp"
#include "operation.hpp"
#include <iostream>
#include <cstdlib> // For exit()

using namespace std;

netlist::netlist(Netlist n) : myNetlist(n) {}

void netlist::refresh() {
    // Cleaning previous memory
    for (auto pair : wire_map) {
        delete pair.second;
    }
    wire_map.clear();
    output_signals.clear();
    input_signals.clear();

    for (auto g : gates) {
        delete g;
    }
    gates.clear();

    // Populating input and outputs
    for (auto inp : myNetlist.inputs) {
        wire* new_wire = new wire(inp);
        input_signals.push_back(new_wire);
        wire_map[inp] = new_wire;
    }
    for (auto out : myNetlist.outputs) {
        wire* new_wire = new wire(out);
        output_signals.push_back(new_wire);
        wire_map[out] = new_wire;
    }

    // Populating internal signals
    for (auto w : myNetlist.wires) {
        wire* new_wire = new wire(w);
        wire_map[w] = new_wire;
    }

    // Creating gates and populating its fields
    int i = 0;
    for (auto gate : myNetlist.gates) {
        vector<wire*> inputs;
        for (auto in : gate.inputs)
            inputs.push_back(wire_map[in]);

        vector<wire*> outputs;
        outputs.push_back(wire_map[gate.output]);
        
        OperationSingleton& singleton = OperationSingleton::getInstance();
        std::shared_ptr<operation> op = singleton.getOperation(gate.type); // Get an operation functor

        node* g = new node(i++, inputs, outputs, op);
        gates.push_back(g);
    }
}

void netlist::simulate(const std::string& s) {
    if (s.size() != input_signals.size()) {
        cerr << "Input vector is not of correct size. " << s.size() << " != " << input_signals.size() << endl;
        exit(1);
    }
    for (size_t i = 0; i < s.size(); i++) {
        input_signals[i]->setLevel(0);
        if (s[i] == '0')
            input_signals[i]->setValue(0);
        else if (s[i] == '1')
            input_signals[i]->setValue(1);
        else {
            cerr << "Input vector can only contain 1's and 0's !" << endl;
            exit(1);
        }
    }

    bool change = true;
    while(change) {
        change = false;
        for (auto g: gates) {
            if (g->evaluated) continue;
            change |= g->eval();
        }
    }
}

void netlist::display_output() const {
    for (auto out : output_signals) {
        cout << out->getName() << " --> " << out->getValue() << endl;
    }
}

netlist::~netlist() {
    // Destructor to clean up dynamically allocated memory
    for (auto pair : wire_map) {
        delete pair.second;
    }
    for (auto g : gates) {
        delete g;
    }
}
