// Maximum Flow using Edmonds-Karp algorithm (BFS-based Ford-Fulkerson) in C++
// Compile: g++ -O2 -std=c++17 maxflow.cpp -o maxflow

#include <bits/stdc++.h>
using namespace std;

class MaxFlow {
    int n;
    vector<vector<int>> capacity; // residual capacity matrix
    vector<vector<int>> adj;      // adjacency list

public:
    MaxFlow(int n) : n(n), capacity(n, vector<int>(n, 0)), adj(n) {}

    // Add a directed edge u -> v with given capacity.
    // If the graph is undirected, add both directions with equal capacity.
    void addEdge(int u, int v, int cap) {
        capacity[u][v] += cap;   // supports multiple edges between same pair
        adj[u].push_back(v);
        adj[v].push_back(u);     // reverse edge (starts with 0 capacity)
    }

    // BFS to find an augmenting path; returns bottleneck capacity (0 if none)
    int bfs(int s, int t, vector<int>& parent) {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = s;
        queue<pair<int,int>> q;
        q.push({s, INT_MAX});

        while (!q.empty()) {
            auto [u, flow] = q.front(); q.pop();

            for (int v : adj[u]) {
                if (parent[v] == -1 && capacity[u][v] > 0) {
                    parent[v] = u;
                    int newFlow = min(flow, capacity[u][v]);
                    if (v == t) return newFlow;
                    q.push({v, newFlow});
                }
            }
        }
        return 0;
    }

    // Compute max flow from source s to sink t
    int maxflow(int s, int t) {
        int flow = 0;
        vector<int> parent(n);
        int newFlow;

        while ((newFlow = bfs(s, t, parent)) != 0) {
            flow += newFlow;
            int cur = t;
            while (cur != s) {
                int prev = parent[cur];
                capacity[prev][cur] -= newFlow; // reduce forward capacity
                capacity[cur][prev] += newFlow; // increase reverse capacity
                cur = prev;
            }
        }
        return flow;
    }
};

int main() {
    int n, e;
    cout << "Enter number of vertices: ";
    cin >> n;
    cout << "Enter number of edges: ";
    cin >> e;

    MaxFlow mf(n);

    cout << "Enter each edge as: u v capacity (0-indexed)\n";
    for (int i = 0; i < e; i++) {
        int u, v, cap;
        cin >> u >> v >> cap;
        mf.addEdge(u, v, cap);
    }

    int s, t;
    cout << "Enter source and sink: ";
    cin >> s >> t;

    cout << "\nMaximum Flow: " << mf.maxflow(s, t) << "\n";

    return 0;
}

/*
Example:
6 vertices (0..5), source=0, sink=5
Edges:
0 1 16
0 2 13
1 2 10
1 3 12
2 1 4
2 4 14
3 2 9
3 5 20
4 3 7
4 5 4

Expected Max Flow: 23
*/