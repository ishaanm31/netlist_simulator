#include "netlist.hpp"
#include "wire.hpp"
#include "operation.hpp"
#include <iostream>
#include <cstdlib> // For exit()

using namespace std;

netlist::netlist(Netlist n) : myNetlist(n) {}

void netlist::refresh() {
}

void netlist::simulate(const std::string& s) {

}

netlist::~netlist() {
    // Destructor to clean up dynamically allocated memory
    for (auto pair : wire_map) {
        delete pair.second;
    }
    for (auto g : gates) {
        delete g;
    }
}
