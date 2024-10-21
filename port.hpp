#ifndef PORT_HPP
#define PORT_HPP

#include <bits/stdc++.h>
#include "node.hpp"

enum _5_value_logic {
    X = -1,
    _0 = 0,
    _1 = 1,
    D,
    D_bar
};

class port {
protected:
    int fault_value, fault_free_value;
    _5_value_logic D_value;
    int level;

    void eval_D_value(){
        if (fault_free_value == fault_value)
            D_value = (_5_value_logic)fault_free_value;
        else if ((fault_value == -1) || (fault_free_value == -1))
            D_value = X;
        else if ((fault_free_value == 0) and (fault_value == 1))
            D_value = D_bar;
        else
            D_value = D;
        return;
    }

public:
    // Constructor
    port() : fault_value(-1), fault_free_value(-1), 
                            D_value(X), level(-1) {}

    // Setter for value
    bool setFaultValue(int val) {
        if (fault_value == val)
            return false;
        fault_value = val;
        eval_D_value();
        return true;
    }

    // Getter for value
    int getFaultValue() const {
        return fault_value;
    }

    // Setter for value
    bool setFaultFreeValue(int val) {
        if (fault_free_value == val)
            return false;
        fault_free_value = val;
        eval_D_value();
        return true;
    }

    // Getter for value
    int getFaultFreeValue() const {
        return fault_free_value;
    }

    _5_value_logic getDvalue() {
        return D_value;
    }

    // Setter for level
    bool setLevel(int val) {
        if (val == level) return false;
        level = val;
        return true;
    }

    // Getter for level
    int getLevel() const {
        return level;
    }
};

class output_port: public port {
protected:
    node *driver_gate;
    wire *driven_wire;
public:
    output_port(node *_driver_gate, wire *_driven_wire): driver_gate(_driver_gate), driven_wire(_driven_wire) {}

    node *getDriverGate() {
        return driver_gate;
    }

    bool setFaultFreeValue(int val) {
        if (fault_free_value == val)
            return false;
        fault_free_value = val;
        eval_D_value();
        driven_wire->updateFaultFreeValue();
        return true;
    }

    bool setFaultValue(int val) {
        if (fault_value == val)
            return false;
        fault_value = val;
        eval_D_value();
        driven_wire->updateFaultValue();
        return true;
    }

    // Setter for level
    bool setLevel(int val) {
        bool change = (level !=val);
        level = val;
        change |= driven_wire->updateLevel();
        return change;
    }
    
    // Dependent gates
    vector<node*> getDependentGates() {
        return driven_wire->getDependentGates();
    }
};

class input_port: public port {
protected:
    node *input_gate;
    wire *driver_wire;
public:
    input_port(node *_g, wire *_w): input_gate(_g), driver_wire(_w) {}

    node *getInputGate() {
        return input_gate;
    }

    wire * getDriverWire() {
        return driver_wire;
    }
};

#endif // PORT_HPP
