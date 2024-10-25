#ifndef NETLIST_HPP
#define NETLIST_HPP

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <set>
#include <unordered_set>
#include "wire.hpp"
#include "operation.hpp"
#include "node.hpp"
#include "netlist_parser.hpp"
#include "port.hpp"


using namespace std;
// Forward declaration of Netlist structure
struct Netlist;

// Graph to represent the circuit connections between gates and flip-flops
class CircuitGraph {
private:
    // Adjacency list to store connections between gates/flip-flops using strings
    unordered_map<string, vector<string>> adjList;
    unordered_set<string> nodes;  // Set of unique nodes (for inputs and outputs)
    unordered_set<string> flipFlops;  // Set of flip-flop outputs

public:
    // Add an edge between two nodes (source -> destination)
    void addEdge(string u, string v) {
        adjList[u].push_back(v);
        nodes.insert(u);
        nodes.insert(v);
    }

    // Mark a node as a flip-flop output
    void addFlipFlop(string output) {
        flipFlops.insert(output);
    }

    // Depth-First Search to find the longest path through flip-flops from a given node
    int dfs(string node, unordered_map<string, int>& visited, unordered_map<string, int>& dp) {
        if (visited[node]) return dp[node];  // If already processed, return depth

        visited[node] = 1;  // Mark node as visited
        int maxDepth = 0;

        // Traverse all neighbors
        for (string neighbor : adjList[node]) {
            int depthThroughNeighbor = dfs(neighbor, visited, dp);
            maxDepth = max(maxDepth, depthThroughNeighbor);
        }

        // If the node is a flip-flop, count it towards the depth
        dp[node] = (flipFlops.find(node) != flipFlops.end()) ? 1 + maxDepth : maxDepth;
        return dp[node];
    }

    // Function to calculate the sequential depth of the circuit
    int calculateSequentialDepth() {
        unordered_map<string, int> visited;  // To mark visited nodes
        unordered_map<string, int> dp;       // To store the depth of each node

        int maxDepth = 0;

        // Run DFS for each node (flip-flop or logic gate)
        for (const string& node : nodes) {
            if (!visited[node]) {
                maxDepth = max(maxDepth, dfs(node, visited, dp));
            }
        }

        return maxDepth;
    }
};


// netlist class definition
class netlist {
private:
    Netlist unrolled_netlist;
    std::map<string ,primary_output_port*> output_signals;
    std::map<string, primary_input_port*> input_signals;
    std::vector<node*> gates;
    std::map<std::string, wire*> wire_map;
    map<string, port*> port_map;
    set<node*> D_frontier;
    int seq_depth;

public:
    // Constructor
    Netlist myNetlist;

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

    void unroll();

    int getSeqDepth(Netlist n);

    // Destructor to clean up allocated resources
    ~netlist();

    void print_netlist(Netlist n);
};

#endif // NETLIST_HPP
