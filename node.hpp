#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include "signal.hpp"
#include "operation.hpp"

using namespace std;

class node {
private:
    int node_idx;
    std::vector<signal*> inputs;
    std::vector<signal*> outputs;
    operation* op;

public:
    // Constructor
    node(int idx, vector<signal*> inp, vector<signal*> out, operation* op_):
        node_idx(idx), inputs(inp), outputs(out), op(op_) {}

    // Evaluate outputs of the module/gate
    // Return true if evaluation is successful
    bool eval() {
        vector<bool> input_vec;
        for (auto inp: inputs) {
            // returns false if any input is uninitialized
            if (inp->getValue() == -1)
                return false;
            input_vec.push_back(inp->getValue());
        }
        vector<bool> output_vec = (*op)(input_vec);

        // Error checking
        if (output_vec.size() != outputs.size()) {
            cout<< "Output vector error on node: "<<node_idx<<endl;
            exit(1);
        }
        // Initializing output signals
        for(int i = 0; i < output_vec.size(); i++) 
            outputs[i]->setValue(output_vec[i]);
        
        return true;
    }
};

#endif