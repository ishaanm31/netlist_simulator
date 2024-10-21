#include "wire.hpp"
#include "port.hpp"
#include "node.hpp"
#include <iostream>

using namespace std;

// Constructor
wire::wire(string _name): driver_port(NULL), name(_name) {}

// Gets us the driver port of this wire. Can only be connected to one output port
output_port* wire::createDriverPort(node *driver_gate) {
    if (driver_port != NULL) {
        cerr << "Tried to create driver port twice on wire: " << name << "\nExiting the system!";
        exit(1);
    }
    driver_port = new output_port(driver_gate, this);
    return driver_port;
}

// Gets us the driver port of this wire. Used for creating primary input
primary_input_port* wire::createDriverPort() {
    if (driver_port != NULL) {
        cerr << "Tried to create driver port twice on wire: " << name << "\nExiting the system!";
        exit(1);
    }
    driver_port = new primary_input_port(this);
    return (primary_input_port*)driver_port;
}

output_port* wire::getDriverPort() {
    return driver_port;
}

// Creates input ports for the gate
input_port* wire::createDrivenPort(node *driven_gate) {
    input_port* ip_port = new input_port(driven_gate, this);
    driven_ports.push_back(ip_port);
    return ip_port;
}

// Creates primary output ports
primary_output_port* wire::createDrivenPort() {
    primary_output_port* ip_port = new primary_output_port(this);
    driven_ports.push_back(ip_port);
    return (primary_output_port*) ip_port;
}

// Updates values of input ports when output port's fault-free value changes
bool wire::updateFaultFreeValue() {
    bool change = false;
    int val = driver_port->getFaultFreeValue();
    for (auto x: driven_ports) {
        change |= x->setFaultFreeValue(val);
    }
    return change;
}

// Updates values of input ports when output port's fault value changes
bool wire::updateFaultValue() {
    bool change = false;
    int val = driver_port->getFaultValue();
    for (auto x: driven_ports) {
        change |= x->setFaultValue(val);
    }
    return change;
}

bool wire::updateLevel() {
    bool change = false;
    int level = driver_port->getLevel();
    for (auto x: driven_ports) {
        change |= x->setLevel(level);
    }
    return change;
}

vector<node*> wire::getDependentGates() {
    vector<node*> V;
    for (auto inp: driven_ports) {
        if (dynamic_cast<primary_output_port*>(inp))
            continue;
        V.push_back(inp->getInputGate());
    }
    return V;
}
