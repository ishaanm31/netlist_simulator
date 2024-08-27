#ifndef OPERATION_HPP
#define OPERATION_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

// Interface for operation
class operation {
public:
    virtual std::vector<bool> operator()(std::vector<bool> input_) const = 0;
    virtual ~operation() = default;  // Virtual destructor for proper cleanup
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

// Singleton Manager for operation functors
class OperationSingleton {
public:
    // Static method to get the singleton instance
    static OperationSingleton& getInstance() {
        static OperationSingleton instance;
        return instance;
    }

    // Method to get a singleton instance of an operation functor by type
    std::shared_ptr<operation> getOperation(const std::string& type) {
        auto it = operations.find(type);
        if (it != operations.end()) {
            return it->second;
        } else {
            std::cerr << "Unknown operation type: " << type << std::endl;
            return nullptr;
        }
    }

    // Prevent copying and assignment
    OperationSingleton(const OperationSingleton&) = delete;
    OperationSingleton& operator=(const OperationSingleton&) = delete;

private:
    // Private constructor for singleton
    OperationSingleton() {
        // Initialize operator functors
        operations["AND2"] = std::make_shared<op_and2>();
        operations["OR2"] = std::make_shared<op_or2>();
        operations["INV"] = std::make_shared<op_not>();
        operations["NAND2"] = std::make_shared<op_nand2>();
        operations["NOR2"] = std::make_shared<op_nor2>();
        operations["XOR2"] = std::make_shared<op_xor2>();
        operations["XNOR2"] = std::make_shared<op_xnor2>();
        operations["BUF"] = std::make_shared<op_buf>();
    }

    // Map of operations, stored as shared_ptr to manage memory
    std::unordered_map<std::string, std::shared_ptr<operation>> operations;
};

#endif // OPERATION_HPP
