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
    std::vector<wire*> inputs;
    std::vector<wire*> outputs;
    std::shared_ptr<operation> op;  // Changed to std::shared_ptr<operation>
    int level;
public:
    bool evaluated;
    // Constructor
    node(int idx, vector<wire*> inp, vector<wire*> out, std::shared_ptr<operation> op_):
        node_idx(idx), inputs(inp), outputs(out), op(op_), evaluated(false) {}

    int getIndex() {
        return node_idx;
    }

    // Evaluate outputs of the module/gate
    // Return true if evaluation is successful
    bool eval() {
        vector<bool> input_vec;
        int max_input_level = -1;
        for (auto inp: inputs) {
            // returns false if any input is uninitialized
            if (inp->getFaultFreeValue() == -1)
                return false;
            input_vec.push_back(inp->getFaultFreeValue());
            max_input_level = max(max_input_level, inp->getLevel());
        }
        vector<bool> output_vec = (*op)(input_vec);
        // Error checking
        if (output_vec.size() != outputs.size()) {
            cerr << "Output vector error on node: " << node_idx << endl;
            exit(1);
        }
        level = 1 + max_input_level;
        // Initializing output signals
        for(long unsigned int i = 0; i < output_vec.size(); i++) {
            outputs[i]->setFaultFreeValue(output_vec[i]);
            outputs[i]->setLevel(level);    
        }
        evaluated = true;
        return true;
    }
};

#endif // NODE_HPP
