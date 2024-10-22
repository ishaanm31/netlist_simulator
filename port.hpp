#ifndef PORT_HPP
#define PORT_HPP

#include <vector>
#include <iostream>  // For std::cerr
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

    // Make functions virtual to allow overriding
    virtual bool setFaultValue(int val);
    virtual void setStuckAtFault(int val);
    virtual void refresh();
    virtual int getFaultValue() const;
    virtual bool setFaultFreeValue(int val);
    virtual int getFaultFreeValue() const;
    virtual _5_value_logic getDvalue();
    virtual bool setLevel(int val);
    virtual int getLevel() const;
};

// Output port class
class output_port : public port {
protected:
    node* driver_gate;
    wire* driven_wire;

public:
    virtual ~output_port() = default;
    output_port(node* _driver_gate, wire* _driven_wire);

    // Override functions
    bool setFaultFreeValue(int val) override;
    bool setFaultValue(int val) override;
    void setStuckAtFault(int val) override;
    bool setLevel(int val) override;
    std::vector<node*> getDependentGates();

    node* getDriverGate();
};

// Input port class
class input_port : public port {
protected:
    node* input_gate;
    wire* driver_wire;

public:
    virtual ~input_port() = default;
    input_port(node* _g, wire* _w);

    // Override functions
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
