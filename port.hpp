#ifndef PORT_HPP
#define PORT_HPP

#include <vector>
class wire;
class node;

enum _5_value_logic {
    X = -1,
    _0 = 0,
    _1 = 1,
    D,
    D_bar
};

// Base class for ports
class port {
protected:
    int fault_value, fault_free_value;
    _5_value_logic D_value;
    int level;
    bool is_stuck;

    void eval_D_value();

public:
    // Add a virtual destructor to make the class polymorphic
    virtual ~port() = default;

    port();

    bool setFaultValue(int val);
    void setStuckAtFault(int val);
    void refresh();
    int getFaultValue() const;
    bool setFaultFreeValue(int val);
    int getFaultFreeValue() const;
    _5_value_logic getDvalue();
    bool setLevel(int val);
    int getLevel() const;
};

// Output port class
class output_port : public port {
protected:
    node* driver_gate;
    wire* driven_wire;

public:
    virtual ~output_port() = default;
    output_port(node* _driver_gate, wire* _driven_wire);
    node* getDriverGate();
    bool setFaultFreeValue(int val);
    bool setFaultValue(int val);
    void setStuckAtFault(int val);
    bool setLevel(int val);
    std::vector<node*> getDependentGates();
};

// Input port class
class input_port : public port {
protected:
    node* input_gate;
    wire* driver_wire;

public:
    virtual ~input_port() = default;
    input_port(node* _g, wire* _w);
    node* getInputGate();
    wire* getDriverWire();
};

// Primary input port class
class primary_input_port : public output_port {
public:
    primary_input_port(wire* _w);
    node* getDriverGate() = delete;
};

// Primary output port class
class primary_output_port : public input_port {
public:
    primary_output_port(wire* _w);
    node* getInputGate() = delete;
};

#endif // PORT_HPP
