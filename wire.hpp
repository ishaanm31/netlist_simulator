#ifndef WIRE_HPP
#define WIRE_HPP

#include <string>

class wire {
private:
    int value;      // Move `value` before `name` in the declaration
    int level;
    std::string name;  // `name` should be declared last

public:
    // Constructor
    wire(std::string name_) : value(-1), level(-1), name(name_) {}

    // Setter for value
    void setValue(int val) {
        value = val;
    }

    // Getter for value
    int getValue() const {
        return value;
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
