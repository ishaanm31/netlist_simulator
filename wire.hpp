#ifndef WIRE_HPP
#define WIRE_HPP

#include <string>

enum _5_value_logic {
    X = -1,
    _0 = 0,
    _1 = 1,
    D,
    D_bar,
};

class wire {
private:
    int fault_value, fault_free_value;
    _5_value_logic D_value;
    int level;
    std::string name;

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
    wire(std::string name_) : fault_value(-1), fault_free_value(-1), 
                            D_value(X), level(-1), name(name_) {}

    // Setter for value
    void setFaultValue(int val) {
        fault_value = val;
        eval_D_value();
    }

    // Getter for value
    int getFaultValue() const {
        return fault_value;
    }

    // Setter for value
    void setFaultFreeValue(int val) {
        fault_free_value = val;
        eval_D_value();
    }

    // Getter for value
    int getFaultFreeValue() const {
        return fault_free_value;
    }

    _5_value_logic getDvalue() {
        return D_value;
    }

    // Setter for level
    void setLevel(int val) {
        level = val;
    }

    // Getter for level
    int getLevel() const {
        return level;
    }

    // Getter for name
    std::string getName() const {
        return name;
    }
};

#endif // WIRE_HPP
