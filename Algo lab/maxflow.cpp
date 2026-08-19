#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int MAX_NODES = 100; // Adjust based on problem constraints
const int INF = 1e9;

int n; // Number of vertices
int capacity[MAX_NODES][MAX_NODES]; // Residual capacity matrix
int parent[MAX_NODES];              // Stores the path found by BFS

// Standard BFS to check if there is an augmenting path from source (s) to sink (t)
bool bfs(int s, int t) {
    // Reset all parent pointers to -1 (unvisited)
    for (int i = 0; i < n; i++) {
        parent[i] = -1;
    }

    queue<int> q;
    q.push(s);
    parent[s] = s; // Mark source as visited

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            // If node v is not visited and there is remaining capacity
            if (parent[v] == -1 && capacity[u][v] > 0) {
                parent[v] = u;
                if (v == t) return true; // Reached the sink
                q.push(v);
            }
        }
    }
    return false; // No path found
}

// Compute Maximum Flow using Edmonds-Karp
int maxFlow(int s, int t) {
    int totalFlow = 0;

    // While an augmenting path exists from s to t
    while (bfs(s, t)) {
        // Step 1: Find the bottleneck (minimum capacity) along the path
        int pathFlow = INF;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            pathFlow = min(pathFlow, capacity[u][v]);
        }

        // Step 2: Update residual capacities
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            capacity[u][v] -= pathFlow; // Reduce forward capacity
            capacity[v][u] += pathFlow; // Increase reverse capacity
        }

        totalFlow += pathFlow;
    }

    return totalFlow;
}

int main() {
    int e;
    cout << "Enter number of vertices: ";
    cin >> n;
    cout << "Enter number of edges: ";
    cin >> e;

    // Initialize capacity matrix with 0
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            capacity[i][j] = 0;
        }
    }

    cout << "Enter each edge as: u v capacity (0-indexed)\n";
    for (int i = 0; i < e; i++) {
        int u, v, cap;
        cin >> u >> v >> cap;
        capacity[u][v] += cap; // Handles multiple edges between the same pair
    }

    int s, t;
    cout << "Enter source and sink: ";
    cin >> s >> t;

    cout << "\nMaximum Flow: " << maxFlow(s, t) << "\n";

    return 0;
}