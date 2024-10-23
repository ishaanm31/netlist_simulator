#include "port.hpp"
#include "wire.hpp"
#include "node.hpp"
#include <iostream>

// Constructor for port class
port::port() : fault_value(-1), fault_free_value(-1), D_value(X), level(-1), is_stuck(false) {}

// Evaluate D value based on fault and fault-free values
void port::eval_D_value() {
    if (fault_free_value == fault_value)
        D_value = (_5_value_logic)fault_free_value;
    else if ((fault_value == -1) || (fault_free_value == -1))
        D_value = X;
    else if ((fault_free_value == 0) && (fault_value == 1))
        D_value = D_bar;
    else
        D_value = D;
}

// Setter for fault value
bool port::setFaultValue(int val) {
    if (is_stuck)
        return false;
    if (fault_value == val)
        return false;
    fault_value = val;
    eval_D_value();
    return true;
}

bool port::getStuckAtFault(){
     return is_stuck;
}

// Set stuck-at fault
void port::setStuckAtFault(int val) {
    is_stuck = true;
    fault_value = val;
    eval_D_value();
}

// Refresh the port values
void port::refresh() {
    fault_free_value = -1;
    fault_value = -1;
    is_stuck = false;
    D_value = X;
}

// Getter for fault value
int port::getFaultValue() const {
    return fault_value;
}

// Setter for fault-free value
bool port::setFaultFreeValue(int val) {
    if (fault_free_value == val)
        return false;
    fault_free_value = val;
    eval_D_value();
    return true;
}

// Getter for fault-free value
int port::getFaultFreeValue() const {
    return fault_free_value;
}

// Get D value
_5_value_logic port::getDvalue() {
    return D_value;
}

// Setter for level
bool port::setLevel(int val) {
    if (val == level)
        return false;
    level = val;
    return true;
}

// Getter for level
int port::getLevel() const {
    return level;
}

// Constructor for output_port
output_port::output_port(node *_driver_gate, wire *_driven_wire) : driver_gate(_driver_gate), driven_wire(_driven_wire) {}

// Set fault-free value for output port
bool output_port::setFaultFreeValue(int val) {
    if (fault_free_value == val)
        return false;
    fault_free_value = val;
    eval_D_value();
    driven_wire->updateFaultFreeValue();
    return true;
}

// Set fault value for output port
bool output_port::setFaultValue(int val) {
    if (is_stuck)
        return false;
    if (fault_value == val)
        return false;
    fault_value = val;
    eval_D_value();
    driven_wire->updateFaultValue();
    return true;
}

// Set stuck-at fault for output port
void output_port::setStuckAtFault(int val) {
    is_stuck = true;
    fault_value = val;
    eval_D_value();
    driven_wire->updateFaultValue();
}

// Set level for output port
bool output_port::setLevel(int val) {
    bool change = (level != val);
    level = val;
    change |= driven_wire->updateLevel();
    return change;
}

// Get dependent gates
std::vector<node*> output_port::getDependentGates() {
    return driven_wire->getDependentGates();
}

// Constructor for input_port
input_port::input_port(node *_g, wire *_w) : input_gate(_g), driver_wire(_w) {}

// Get input gate
node *input_port::getInputGate() {
    if (input_gate == nullptr) {
        std::cerr << "Input gate not defined" << std::endl;
        exit(1);
    }
    return input_gate;
}

// Get driver wire
wire *input_port::getDriverWire() {
    return driver_wire;
}

// Constructor for primary_input_port
primary_input_port::primary_input_port(wire *_w) : output_port(nullptr, _w) {}

// Constructor for primary_output_port
primary_output_port::primary_output_port(wire *_w) : input_port(nullptr, _w) {}
