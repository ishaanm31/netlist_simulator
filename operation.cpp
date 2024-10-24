#include "operation.hpp"
#include <iostream>
#include <cstdlib>  // For exit()

// AND gate
op_and2::op_and2() { inversion_parity = 0; }
int op_and2::operator()(const std::vector<int>& input_) const {
    if (input_.size() != 2) {
        std::cerr << "Input vector of AND2 gate must have exactly 2 inputs" << std::endl;
        exit(1);
    }
    if (input_[0] == 0 || input_[1] == 0) {
        return 0;  // Controlling case for AND
    }
    if (input_[0] == -1 || input_[1] == -1) {
        return -1; // Unknown result
    }
    return (input_[0] && input_[1]) ? 1 : 0;
}

// OR gate
op_or2::op_or2() { inversion_parity = 0; }
int op_or2::operator()(const std::vector<int>& input_) const {
    if (input_.size() != 2) {
        std::cerr << "Input vector of OR2 gate must have exactly 2 inputs" << std::endl;
        exit(1);
    }
    if (input_[0] == 1 || input_[1] == 1) {
        return 1;  // Controlling case for OR
    }
    if (input_[0] == -1 || input_[1] == -1) {
        return -1; // Unknown result
    }
    return (input_[0] || input_[1]) ? 1 : 0;
}

// NOT gate
op_not::op_not() { inversion_parity = 1; }
int op_not::operator()(const std::vector<int>& input_) const {
    if (input_.size() != 1) {
        std::cerr << "Input vector of NOT gate must have exactly 1 input" << std::endl;
        exit(1);
    }
    if (input_[0] == -1) {
        return -1;  // Unknown result
    }
    return !input_[0] ? 1 : 0;
}

// NAND gate
op_nand2::op_nand2() { inversion_parity = 1; }
int op_nand2::operator()(const std::vector<int>& input_) const {
    if (input_.size() != 2) {
        std::cerr << "Input vector of NAND2 gate must have exactly 2 inputs" << std::endl;
        exit(1);
    }
    if (input_[0] == 0 || input_[1] == 0) {
        return 1;  // Controlling case for NAND
    }
    if (input_[0] == -1 || input_[1] == -1) {
        return -1; // Unknown result
    }
    return !(input_[0] && input_[1]) ? 1 : 0;
}

// NOR gate
op_nor2::op_nor2() { inversion_parity = 1; }
int op_nor2::operator()(const std::vector<int>& input_) const {
    if (input_.size() != 2) {
        std::cerr << "Input vector of NOR2 gate must have exactly 2 inputs" << std::endl;
        exit(1);
    }
    if (input_[0] == 1 || input_[1] == 1) {
        return 0;  // Controlling case for NOR
    }
    if (input_[0] == -1 || input_[1] == -1) {
        return -1; // Unknown result
    }
    return !(input_[0] || input_[1]) ? 1 : 0;
}

// XOR gate
op_xor2::op_xor2() { inversion_parity = 0; }
int op_xor2::operator()(const std::vector<int>& input_) const {
    if (input_.size() != 2) {
        std::cerr << "Input vector of XOR2 gate must have exactly 2 inputs" << std::endl;
        exit(1);
    }
    if (input_[0] == -1 || input_[1] == -1) {
        return -1; // Unknown result
    }
    return (input_[0] != input_[1]) ? 1 : 0;
}

// XNOR gate
op_xnor2::op_xnor2() { inversion_parity = 1; }
int op_xnor2::operator()(const std::vector<int>& input_) const {
    if (input_.size() != 2) {
        std::cerr << "Input vector of XNOR2 gate must have exactly 2 inputs" << std::endl;
        exit(1);
    }
    if (input_[0] == -1 || input_[1] == -1) {
        return -1; // Unknown result
    }
    return (input_[0] == input_[1]) ? 1 : 0;
}

// Buffer gate
op_buf::op_buf() { inversion_parity = 0; }
int op_buf::operator()(const std::vector<int>& input_) const {
    if (input_.size() != 1) {
        std::cerr << "Input vector of BUF gate must have exactly 1 input" << std::endl;
        exit(1);
    }
    if (input_[0] == -1) {
        return -1;  // Unknown result
    }
    return input_[0];
}


// Singleton Manager for operation functors
OperationSingleton::OperationSingleton() {
    operations["AND2"] = std::make_shared<op_and2>();
    operations["OR2"] = std::make_shared<op_or2>();
    operations["INV"] = std::make_shared<op_not>();
    operations["NAND2"] = std::make_shared<op_nand2>();
    operations["NOR2"] = std::make_shared<op_nor2>();
    operations["XOR2"] = std::make_shared<op_xor2>();
    operations["XNOR2"] = std::make_shared<op_xnor2>();
    operations["BUF"] = std::make_shared<op_buf>();
}

OperationSingleton& OperationSingleton::getInstance() {
    static OperationSingleton instance;
    return instance;
}

std::shared_ptr<operation> OperationSingleton::getOperation(const std::string& type) {
    auto it = operations.find(type);
    if (it != operations.end()) {
        return it->second;
    } else {
        std::cerr << "Unknown operation type: " << type << std::endl;
        return nullptr;
    }
}
