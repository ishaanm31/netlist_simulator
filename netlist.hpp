#ifndef NETLIST_HPP
#define NETLIST_HPP

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <set>
#include "wire.hpp"
#include "operation.hpp"
#include "node.hpp"
#include "netlist_parser.hpp"

// Forward declaration of Netlist structure
struct Netlist;

// netlist class definition
class netlist {
private:
    Netlist myNetlist;
    std::vector<wire*> output_signals;
    std::vector<wire*> input_signals;
    std::vector<node*> gates;
    std::map<std::string, wire*> wire_map;

public:
    // Constructor
    netlist(Netlist n);

    // Populates internal fields using the base netlist
    // Must be called before reusing the netlist for new input vector
    void refresh();

    // Simulates with input and stores the values in output
    void simulate(const std::string& s);

    // Displays the output signals
    void display_output() const;

    // Destructor to clean up allocated resources
    ~netlist();
};

#endif // NETLIST_HPP
