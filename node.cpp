#include "node.hpp"
#include "wire.hpp"
#include "port.hpp"
#include "operation.hpp"
#include <iostream>

// Constructor for node
node::node(int idx, std::vector<wire*> input_wires, wire* output_wire, std::string gate_type)
    : node_idx(idx), evaluated(false) {
    gate_type = gate_type;
    OperationSingleton& singleton = OperationSingleton::getInstance();
    op = singleton.getOperation(gate_type); // Get an operation functor
    
    for (auto input_wire : input_wires) {
        inputs.push_back(input_wire->createDrivenPort(this));
    }
    
    output = output_wire->createDriverPort(this);
}

// Getter for node index
int node::getIndex() {
    return node_idx;
}

int node::getLevel() {
    return level;
}

// Evaluate outputs of the module/gate, return gates which need to be re-evaluated
std::vector<node*> node::eval() {
    bool change = false;
    std::vector<int> input_vec_f_free, input_vec_f;
    
    // Fetch input values from input ports
    for (auto input_port : inputs) {
        int fault_free_value = input_port->getFaultFreeValue();
        int fault_value = input_port->getFaultValue();
        input_vec_f_free.push_back(fault_free_value);
        input_vec_f.push_back(fault_value);
    }

    // Evaluate new output values
    int new_f_free = (*op)(input_vec_f_free);
    int new_f = (*op)(input_vec_f);
    int prev_f_free = output->getFaultFreeValue();
    int prev_f = output->getFaultValue();

    // Check if output has changed
    if (prev_f_free != new_f_free) {
        change = true;
        output->setFaultFreeValue(new_f_free);
    }
    if (prev_f != new_f) {
        change = true;
        output->setFaultValue(new_f);
    }

    // If no change, return empty vector
    if (!change)
        return {};

    // Return dependent gates that need to be re-evaluated
    return output->getDependentGates();
}

// Evaluate the level of the gate, return gates which need to be re-evaluated
std::vector<node*> node::evalLevel() {
    int max_inp_level = -1;
    // Find the maximum level among inputs
    for (auto inp : inputs) {
        max_inp_level = std::max(max_inp_level, inp->getLevel());
    }
    // If level remains the same, return empty vector
    if (max_inp_level == level)
        return {};


    // Update level and propagate changes to output
    level = max_inp_level + 1;
    output->setLevel(level);

    // Return dependent gates
    return output->getDependentGates();
}
