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
    virtual int operator()(const std::vector<int>& input_) const = 0;
    virtual ~operation() = default;  // Virtual destructor for proper cleanup
    int getInversionParity() const { return inversion_parity; }

protected:
    int inversion_parity = 0;  // Default is 0 (no inversion)
};

// AND gate
class op_and2 : public operation {
public:
    op_and2();
    int operator()(const std::vector<int>& input_) const override;
};

// OR gate
class op_or2 : public operation {
public:
    op_or2();
    int operator()(const std::vector<int>& input_) const override;
};

// NOT gate
class op_not : public operation {
public:
    op_not();
    int operator()(const std::vector<int>& input_) const override;
};

// NAND gate
class op_nand2 : public operation {
public:
    op_nand2();
    int operator()(const std::vector<int>& input_) const override;
};

// NOR gate
class op_nor2 : public operation {
public:
    op_nor2();
    int operator()(const std::vector<int>& input_) const override;
};

// XOR gate
class op_xor2 : public operation {
public:
    op_xor2();
    int operator()(const std::vector<int>& input_) const override;
};

// XNOR gate
class op_xnor2 : public operation {
public:
    op_xnor2();
    int operator()(const std::vector<int>& input_) const override;
};

// Buffer gate
class op_buf : public operation {
public:
    op_buf();
    int operator()(const std::vector<int>& input_) const override;
};

// Singleton Manager for operation functors
class OperationSingleton {
public:
    static OperationSingleton& getInstance();

    std::shared_ptr<operation> getOperation(const std::string& type);

    OperationSingleton(const OperationSingleton&) = delete;
    OperationSingleton& operator=(const OperationSingleton&) = delete;

private:
    OperationSingleton();
    std::unordered_map<std::string, std::shared_ptr<operation>> operations;
};

#endif // OPERATION_HPP
