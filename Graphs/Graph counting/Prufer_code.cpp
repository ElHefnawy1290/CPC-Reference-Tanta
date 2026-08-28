#include <bits/stdc++.h>
using namespace std;

/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: PRUFER CODE
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Labeled Trees", "Cayley's Formula", "Tree Counting".
 * - Classic Scenarios: Bijectively mapping a labeled tree to a sequence of length N-2.
 * - The Magic: It allows you to count trees with specific constraints (like degrees)
 *   by counting sequences instead, which is mathematically trivial.
 */

struct Prufer
{
    // Converts a labeled tree (adj list) to a Prufer sequence of length N-2
    // Complexity: O(N log N)
    static vector<int> tree_to_prufer(int n, vector<vector<int>> &adj)
    {
        if (n == 2)
            return {}; // A 2-node tree has no Prufer sequence

        vector<int> degree(n + 1);
        priority_queue<int, vector<int>, greater<int>> leaves;

        for (int i = 1; i <= n; i++)
        {
            degree[i] = adj[i].size();
            if (degree[i] == 1)
                leaves.push(i);
        }

        vector<int> prufer;
        vector<bool> removed(n + 1, false);

        for (int i = 0; i < n - 2; i++)
        {
            int leaf = leaves.top();
            leaves.pop();
            removed[leaf] = true;

            // Find the neighbor of this leaf
            int neighbor = -1;
            for (int v : adj[leaf])
            {
                if (!removed[v])
                {
                    neighbor = v;
                    break;
                }
            }

            prufer.push_back(neighbor);
            degree[neighbor]--;
            if (degree[neighbor] == 1)
                leaves.push(neighbor);
        }

        return prufer;
    }

    // Converts a Prufer sequence of length N-2 back to an edge list of the tree
    // Complexity: O(N log N)
    static vector<pair<int, int>> prufer_to_tree(int n, const vector<int> &prufer)
    {
        vector<int> degree(n + 1, 1);
        for (int x : prufer)
            degree[x]++;

        priority_queue<int, vector<int>, greater<int>> leaves;
        for (int i = 1; i <= n; i++)
        {
            if (degree[i] == 1)
                leaves.push(i);
        }

        vector<pair<int, int>> edges;
        for (int x : prufer)
        {
            int leaf = leaves.top();
            leaves.pop();
            edges.push_back({leaf, x});

            degree[x]--;
            if (degree[x] == 1)
                leaves.push(x);
        }

        // Connect the last two remaining nodes
        int u = leaves.top();
        leaves.pop();
        int v = leaves.top();
        leaves.pop();
        edges.push_back({u, v});

        return edges;
    }
};

void solve()
{
    int n;
    cin >> n;
    // Example: Tree to Prufer
    // vector<vector<int>> adj(n+1);
    // ... fill adj ...
    // vector<int> p = Prufer::tree_to_prufer(n, adj);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    return 0;
}