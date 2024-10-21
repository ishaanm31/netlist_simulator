#ifndef WIRE_HPP
#define WIRE_HPP

#include <string>
#include "port.hpp"
#include <bits/stdc++.h>
using namespace std;

// Connects output ports to the corresponding input ports
// Call this wire to set ports of the gates
class wire {
private:
    output_port *driver_port;
    vector<input_port*> driven_ports;
    string name;
    
public:
    wire(string _name):driver_port(NULL), name(_name){}

    // Gets us the driver port of this wire. Can be only 
    // connected to one output ports. So should be called only once
    output_port *createDriverPort(node *driver_gate) {
        if (driver_port != NULL) {
            cerr<<"Tried to create driver port twice on wire: "<<name<<"\nExiting the system!";
            exit(1);
        }
        driver_port = new output_port(driver_gate, this);
        return driver_port;
    }
    output_port *getDriverPort() {
        return driver_port;
    }

    // Creates input ports for the gate
    input_port *createDrivenPort(node *driven_gate) {
        input_port* ip_port = new input_port(driven_gate, this);
        driven_ports.push_back(ip_port);
        return ip_port;
    }

    // Everytime someone updates the value of the output port
    // This function is called to update values of input ports
    bool updateFaultFreeValue() {
        bool change = false;
        int val = driver_port->getFaultFreeValue();
        for (auto x: driven_ports) {
            change |= x->setFaultFreeValue(val);
        }
        return change;
    }

    // Everytime someone updates the value of the output port
    // This function is called to update values of input ports
    bool updateFaultValue() {
        bool change = false;
        int val = driver_port->getFaultValue();
        for (auto x: driven_ports) {
            change |= x->setFaultValue(val);
        }
        return change;
    }
    bool updateLevel() {
        bool change = false;
        int level = driver_port->getLevel();
        for (auto x: driven_ports) {
            change |= x->setLevel(level);
        }
        return change;
    }
    vector<node*> getDependentGates() {
        vector<node*> V;
        for (auto inp: driven_ports) {
            V.push_back(inp->getInputGate());
        }
        return V;
    }
};
#endif // WIRE_HPP
