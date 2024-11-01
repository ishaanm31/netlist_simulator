#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <memory>
#include <string>
#include "wire.hpp"
#include "port.hpp"
#include "operation.hpp"

// Forward declarations
class wire;
class port;
class output_port;
class input_port;
class primary_input_port;
class primary_output_port;
class operation;

// Represents a gate
class node {
private:
    int node_idx;
    std::vector<input_port*> inputs;
    std::vector<input_port*> PPI;
    output_port* output;
    std::shared_ptr<operation> op;
    int level;
    std::string gate_type;

public:
    bool evaluated;

    // Constructor
    node(int idx, std::vector<wire*> input_wires, wire* output_wire, std::string gate_type);

    // Getter for node index
    int getIndex();

    // Evaluate outputs of the module/gate, return gates which need to be re-evaluated
    std::vector<node*> eval();

    // Evaluate the level of the gate, return gates which need to be re-evaluated
    std::vector<node*> evalLevel();

    // Get level
    int getLevel();

    // Get input ports
    vector<input_port*> getInputPorts() {
        return inputs;
    }

    // Get output ports
    output_port* getOutputPort() {
        return output;
    }

    // Get gate type
    std::string getGateType(){
        return gate_type;
    };

    // Get inversion parity
    int getInversionParity(){
        return op->getInversionParity();
    };

    bool belong_to_D_frontier();
};

class node_ff{
    


};

#endif // NODE_HPP
