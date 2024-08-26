#ifndef SIGNAL_H
#define SIGNAL_H

#include <string>

class signal {
private:
    int value;
    std::string name;

public:
    // Constructor
    signal(std::string name_) : name(name_), value(-1) {}

    // Setter for value
    void setValue(int val) {
        value = val;
    }

    // Getter for value
    int getValue() const {
        return value;
    }

    // Getter for name
    std::string getName() const {
        return name;
    }
};

#endif // SIGNAL_H