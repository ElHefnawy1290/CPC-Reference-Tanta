#include <bits/stdc++.h>
using namespace std;

/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: DIRECTED MST (CHU-LIU/EDMONDS)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Minimum Spanning Arborescence", "Directed MST".
 * - Classic Scenarios: Finding the cheapest way to connect all nodes in a DIRECTED graph
 *   rooted at a specific node, such that every node (except the root) has exactly 1 incoming edge.
 * - The Magic: It greedily picks the minimum incoming edge for every node. If this creates
 *   cycles, it "contracts" those cycles into a single super-node, updates the weights of edges
 *   entering the cycle, and recurses.
 *
 * 2. 📦 HOW TO USE IT
 * - edge: {u, v, cost}
 * - roots: The node that acts as the root of the arborescence.
 * - Complexity: O(V * E)
 */

struct Edge
{
    int u, v;
    long long cost;
};

long long edmonds(int n, int root, vector<Edge> &edges)
{
    long long total_cost = 0;
    vector<int> min_edge(n + 1);
    vector<int> parent(n + 1);
    vector<int> id(n + 1);
    vector<int> vis(n + 1);

    while (true)
    {
        fill(min_edge.begin(), min_edge.end(), 2e18); // Use large INF
        for (const auto &e : edges)
        {
            if (e.u != e.v && e.cost < min_edge[e.v])
            {
                min_edge[e.v] = e.cost;
                parent[e.v] = e.u;
            }
        }

        for (int i = 1; i <= n; i++)
        {
            if (i != root && min_edge[i] == 2e18)
                return -1; // Impossible
        }

        int cnt_nodes = 0;
        fill(id.begin(), id.end(), -1);
        fill(vis.begin(), vis.end(), -1);
        min_edge[root] = 0;

        for (int i = 1; i <= n; i++)
        {
            total_cost += min_edge[i];
            int v = i;
            while (vis[v] != i && id[v] == -1 && v != root)
            {
                vis[v] = i;
                v = parent[v];
            }
            if (v != root && id[v] == -1)
            {
                for (int u = parent[v]; u != v; u = parent[u])
                    id[u] = cnt_nodes;
                id[v] = cnt_nodes++;
            }
        }

        if (cnt_nodes == 0)
            break;
        for (int i = 1; i <= n; i++)
        {
            if (id[i] == -1)
                id[i] = cnt_nodes++;
        }

        for (auto &e : edges)
        {
            long long v_cost = min_edge[e.v];
            e.u = id[e.u];
            e.v = id[e.v];
            if (e.u != e.v)
                e.cost -= v_cost;
        }
        n = cnt_nodes;
        root = id[root];
    }
    return total_cost;
}