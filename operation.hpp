#ifndef OPERATION_HPP
#define OPERATION_HPP

#include <iostream>
#include <vector>
#include <cstdlib> // For exit()

// Interface for operation
class operation {
public:
    virtual std::vector<bool> operator()(std::vector<bool> input_) const = 0;
};

// AND gate
class op_and2 : public operation {
public:
    std::vector<bool> operator()(std::vector<bool> input_) const override {
        if (input_.size() != 2) {
            std::cerr << "Input vector of AND2 gate must have exactly 2 inputs" << std::endl;
            exit(1);
        }
        return std::vector<bool>{input_[0] && input_[1]};
    }
};

// OR gate
class op_or2 : public operation {
public:
    std::vector<bool> operator()(std::vector<bool> input_) const override {
        if (input_.size() != 2) {
            std::cerr << "Input vector of OR2 gate must have exactly 2 inputs" << std::endl;
            exit(1);
        }
        return std::vector<bool>{input_[0] || input_[1]};
    }
};

// NOT gate
class op_not : public operation {
public:
    std::vector<bool> operator()(std::vector<bool> input_) const override {
        if (input_.size() != 1) {
            std::cerr << "Input vector of NOT gate must have exactly 1 input" << std::endl;
            exit(1);
        }
        return std::vector<bool>{!input_[0]};
    }
};

// NAND gate
class op_nand2 : public operation {
public:
    std::vector<bool> operator()(std::vector<bool> input_) const override {
        if (input_.size() != 2) {
            std::cerr << "Input vector of NAND2 gate must have exactly 2 inputs" << std::endl;
            exit(1);
        }
        return std::vector<bool>{!(input_[0] && input_[1])};
    }
};

// NOR gate
class op_nor2 : public operation {
public:
    std::vector<bool> operator()(std::vector<bool> input_) const override {
        if (input_.size() != 2) {
            std::cerr << "Input vector of NOR2 gate must have exactly 2 inputs" << std::endl;
            exit(1);
        }
        return std::vector<bool>{!(input_[0] || input_[1])};
    }
};

// XOR gate
class op_xor2 : public operation {
public:
    std::vector<bool> operator()(std::vector<bool> input_) const override {
        if (input_.size() != 2) {
            std::cerr << "Input vector of XOR2 gate must have exactly 2 inputs" << std::endl;
            exit(1);
        }
        return std::vector<bool>{input_[0] != input_[1]};
    }
};

// XNOR gate
class op_xnor2 : public operation {
public:
    std::vector<bool> operator()(std::vector<bool> input_) const override {
        if (input_.size() != 2) {
            std::cerr << "Input vector of XNOR2 gate must have exactly 2 inputs" << std::endl;
            exit(1);
        }
        return std::vector<bool>{input_[0] == input_[1]};
    }
};

// Buffer gate
class op_buf : public operation {
public:
    std::vector<bool> operator()(std::vector<bool> input_) const override {
        if (input_.size() != 1) {
            std::cerr << "Input vector of BUF gate must have exactly 1 input" << std::endl;
            exit(1);
        }
        return std::vector<bool>{input_[0]};
    }
};

#endif // OPERATION_HPP
