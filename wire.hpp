#ifndef WIRE_HPP
#define WIRE_HPP

#include <string>
#include <vector>

using namespace std;

// Forward declarations to avoid circular dependencies
class port;
class output_port;
class input_port;
class primary_input_port;
class primary_output_port;
class p_primary_output_port;
class p_primary_input_port;
class node;

// Connects output ports to the corresponding input ports
class wire {
private:
    output_port *driver_port;
    vector<input_port*> driven_ports;
    string name;

public:
    // Constructor
    wire(string _name);

    // Gets us the driver port of this wire. Can only be connected to one output port
    output_port *createDriverPort(node *driver_gate);

    // Gets us the driver port of this wire. Used for creating primary input
    primary_input_port *createDriverPort();
    p_primary_input_port *createDriverPort_();

    output_port *getDriverPort();

    // Creates input ports for the gate
    input_port *createDrivenPort(node *driven_gate);

    // Creates primary output ports
    primary_output_port *createDrivenPort();
    p_primary_output_port *createDrivenPort_();

    // Every time someone updates the value of the output port, this function updates values of input ports
    bool updateFaultFreeValue();

    // Every time someone updates the value of the output port, this function updates values of input ports
    bool updateFaultValue();

    bool updateLevel();

    vector<node*> getDependentGates();
};

#endif // WIRE_HPP
