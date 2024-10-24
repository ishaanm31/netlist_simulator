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
#include "port.hpp"


using namespace std;
// Forward declaration of Netlist structure
struct Netlist;

// netlist class definition
class netlist {
private:
    Netlist myNetlist;
    std::map<string ,primary_output_port*> output_signals;
    std::map<string, primary_input_port*> input_signals;
    std::vector<node*> gates;
    std::map<std::string, wire*> wire_map;
    map<string, port*> port_map;
    set<node*> D_frontier;

public:
    // Constructor
    netlist(Netlist n);

    // Populates internal fields using the base netlist
    // Must be called before reusing the netlist for new input vector
    void refresh();

    // Simulates the netlist
    void simulate();

    void setPI(string PI_name, int val);

    // Displays the output signals
    void display_output() const;

    void levelize();

    // Get test vector for testing all the Stuck at Faults
    map<string, map<string, int>> comb_atpg();

    // bool X_path_check(port* stuck_port);

    pair<primary_input_port*, int> backtrace(pair<port*, int> objective);

    bool podem_recursion(port* stuck_port);

    pair<port*, int> getObjective(port* stuck_port);

    // Destructor to clean up allocated resources
    ~netlist();
};

#endif // NETLIST_HPP
