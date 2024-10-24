#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Graph to represent the flip-flop connections
class FlipFlopGraph {
private:
    // Adjacency list to store connections between flip-flops
    unordered_map<int, vector<int>> adjList;
    // Number of flip-flops in the graph
    int numFlipFlops;

public:
    FlipFlopGraph(int n) : numFlipFlops(n) {}

    // Add an edge between two flip-flops (u -> v)
    void addEdge(int u, int v) {
        adjList[u].push_back(v);
    }

    // Depth-First Search to find the longest path from a given node
    int dfs(int node, vector<int>& visited, vector<int>& dp) {
        // If node is already processed, return its depth
        if (visited[node]) return dp[node];

        // Mark the node as visited
        visited[node] = 1;
        int maxDepth = 0;

        // Traverse all neighbors
        for (int neighbor : adjList[node]) {
            maxDepth = max(maxDepth, dfs(neighbor, visited, dp));
        }

        // Store the longest path from this node
        dp[node] = 1 + maxDepth;
        return dp[node];
    }

    // Function to calculate the sequential depth of the graph
    int calculateSequentialDepth() {
        vector<int> visited(numFlipFlops, 0); // To mark visited nodes
        vector<int> dp(numFlipFlops, 0);      // To store the depth of each node

        int maxDepth = 0;

        // Run DFS for each flip-flop
        for (int i = 0; i < numFlipFlops; i++) {
            if (!visited[i]) {
                maxDepth = max(maxDepth, dfs(i, visited, dp));
            }
        }

        return maxDepth;
    }
};

int main() {
    // Example: 3 flip-flops with connections:
    // FF1 -> FF2 -> FF3
    // Sequential depth should be 3.

    int numFlipFlops = 3;
    FlipFlopGraph graph(numFlipFlops);

    // Adding connections between flip-flops
    graph.addEdge(0, 1);  // FF1 -> FF2
    graph.addEdge(1, 2);  // FF2 -> FF3

    // Calculate the sequential depth
    int depth = graph.calculateSequentialDepth();

    cout << "Sequential Depth: " << depth << endl;

    return 0;
}
