#include <bits/stdc++.h>
using namespace std;

/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: BITSET REACHABILITY
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Transitive Closure", "DAG Reachability", "Bitset Optimization".
 * - Classic Scenarios: You have a Directed Acyclic Graph (DAG) and need to answer Q
 *   queries of the form "Can I reach node Y from node X?".
 * - The Magic: Since a DAG has a topological order, we process nodes in REVERSE
 *   topological order. For each node U, its reachability set is the Bitwise OR
 *   of the reachability sets of all its neighbors. Because bitsets are 64x faster than
 *   arrays, this brings an O(N^3) problem down to an O(N^3 / 64) complexity.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization:
 *       compute_reachability(N); // Run this once after building adjacency list
 *
 * - Query:
 *       if (reach[start].test(target)) { ... }
 *
 * - Complexity:
 *       Time: O(N * (N + M) / 64).
 *       Space: O(N^2 / 8) bytes. (For N=20,000, this is ~50MB).
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Memory Limit Warning:
 *   The current `MAXN = 20005` uses ~50MB. If N > 20,000, you will exceed standard 256MB limits.
 *   If N > 20,000, you MUST use the "Block Reachability" approach (process bits in
 *   chunks of 1024 or 2048) rather than holding the entire N x N matrix in memory.
 */

// ? Dial 1: Adjust MAXN based on memory limits (N^2 / 8 bytes)
const int MAXN = 20005;

// reach[u][v] = 1 if there is a path from u to v
bitset<MAXN> reach[MAXN];
vector<int> adj[MAXN];
vector<int> topo_order;
int in_degree[MAXN];

// Standard Kahn's Algorithm for Topological Sort
void compute_topological_sort(int n)
{
    queue<int> q;
    for (int i = 1; i <= n; i++)
    {
        if (in_degree[i] == 0)
            q.push(i);
    }

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        topo_order.push_back(u);

        for (int v : adj[u])
        {
            in_degree[v]--;
            if (in_degree[v] == 0)
                q.push(v);
        }
    }
}

void compute_reachability(int n)
{
    compute_topological_sort(n);

    // ? THE MAGIC: Process in REVERSE topological order
    // This ensures that when we process node u, all its neighbors v
    // are already fully computed.
    for (int i = n - 1; i >= 0; i--)
    {
        int u = topo_order[i];

        reach[u].set(u); // A node can always reach itself

        for (int v : adj[u])
        {
            // Bitwise OR is the secret sauce:
            // This ORs all bits of reach[v] into reach[u] at once
            reach[u] |= reach[v];
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;

    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        in_degree[v]++;
    }

    compute_reachability(n);

    // Example Query: Can I reach node 'target' from node 'start'?
    int start, target;
    cin >> start >> target;

    if (reach[start].test(target))
    {
        cout << "Reachable!\n";
    }
    else
    {
        cout << "Not reachable.\n";
    }

    return 0;
}