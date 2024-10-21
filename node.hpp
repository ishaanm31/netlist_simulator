#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <memory>  // For std::shared_ptr
#include "wire.hpp"
#include "operation.hpp"

using namespace std;

class node { // Represents a gate
private:
    int node_idx;
    std::vector<input_port*> inputs;
    output_port* output;
    std::shared_ptr<operation> op;  // Changed to std::shared_ptr<operation>
    int level;
public:
    bool evaluated;
    // Constructor
    node(int idx, vector<wire*> input_wires, wire* output_wire, string gate_type):
        node_idx(idx), evaluated(false) {
        
        OperationSingleton& singleton = OperationSingleton::getInstance();
        op = singleton.getOperation(gate_type); // Get an operation functor
        for (auto input_wire: input_wires) {
            inputs.push_back(input_wire->createDrivenPort(this));
        }
        output = output_wire->createDriverPort(this);
    }
    int getIndex() {
        return node_idx;
    }

    // Evaluate outputs of the module/gate
    // Return gates which need to be re-evaluated
    vector<node*> eval() {
        bool change = false;
        // Fetching input vectors from input ports
        vector<int> input_vec_f_free, input_vec_f;
        for (auto input_port: inputs) {
            int fault_free_value = input_port->getFaultFreeValue();
            int fault_value = input_port->getFaultValue();
            input_vec_f_free.push_back(fault_free_value);
            input_vec_f.push_back(fault_value);
        }
        // Evaluating new output values and previous values
        int new_f_free = (*op)(input_vec_f_free), new_f = (*op)(input_vec_f);
        int prev_f_free = output->getFaultFreeValue(), prev_f = output->getFaultValue();

        if (prev_f_free != new_f_free) {
            change = true;
            output->setFaultFreeValue(new_f_free); // Reflecting new value if changes
        }
        if (prev_f != new_f) {
            change = true;
            output->setFaultValue(new_f); // Reflecting new value if changes
        }
        if (!change)
            return {};
        return output->getDependentGates(); // Return dependent gates if there is a change
    }
};

#endif // NODE_HPP
