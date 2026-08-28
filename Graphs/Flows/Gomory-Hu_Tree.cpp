#include <bits/stdc++.h>
using namespace std;

/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: GOMORY-HU TREE
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "All-pairs Min-Cut", "Minimum cut tree", "Undirected Graph Cut".
 * - Classic Scenarios: You have an undirected graph and need to know the minimum
 *   cut between *every* possible pair of vertices.
 * - The Magic: Normally, finding the min-cut between all pairs requires O(N^2)
 *   Max Flow calls. The Gomory-Hu Tree (specifically Gusfield's algorithm implementation)
 *   builds a tree representing all min-cuts using only exactly N-1 Max Flow calls!
 *   The min-cut between any two nodes (u, v) is simply the minimum weight edge on
 *   the path between them in this resulting tree.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization:
 *       GomoryHu gh(N); // N is the number of nodes (0-indexed)
 * - Add Edge:
 *       gh.add_edge(u, v, cap); // Undirected edge
 * - Build:
 *       gh.build(); // Runs the N-1 Max Flow calls to construct the tree
 * - Query:
 *       long long ans = gh.min_cut(u, v);
 *
 * - Complexity:
 *       Time: O(N * MaxFlow(V, E)) to build. O(N) per query (or O(1) if you add LCA).
 *       Space: O(N + E)
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Dinic Dependency: This template assumes you have your `Dinic` struct available
 *   in the same file or included. Ensure `Dinic` has `add_edge(u, v, cap)` and `max_flow(s, t)`.
 * - 1-based indexing: If your nodes are 1 to N, instantiate `GomoryHu gh(N + 1)` and
 *   just ignore index 0.
 */

struct GomoryHu
{
    int n;
    struct Edge
    {
        int u, v;
        long long cap;
    };
    vector<Edge> edges;

    // Parent array for the tree (Gomory-Hu Tree structure)
    vector<int> p;
    // Weight array (min-cut value between i and p[i])
    vector<long long> weight;
    // Depth array for simple O(N) queries without complex LCA
    vector<int> depth;

    GomoryHu(int n) : n(n), p(n), weight(n), depth(n, 0) {}

    void add_edge(int u, int v, long long cap)
    {
        edges.push_back({u, v, cap});
    }

    void build()
    {
        // 1. Initialize parents: everyone initially points to node 0
        fill(p.begin(), p.end(), 0);
        fill(weight.begin(), weight.end(), 0);

        // 2. Iterate to build the tree (Gusfield's Algorithm)
        for (int i = 1; i < n; i++)
        {
            // We rebuild the graph for Dinic each time.
            // (Assuming `Dinic` is your standard Dinic template)
            Dinic dinic(n);
            for (auto &e : edges)
            {
                dinic.add_edge(e.u, e.v, e.cap);
                dinic.add_edge(e.v, e.u, e.cap); // Gomory-Hu requires Undirected edges!
            }

            // Find min s-t cut between current node `i` and its parent `p[i]`
            long long flow = dinic.max_flow(i, p[i]);
            weight[i] = flow;

            // Find which nodes are on the source side of the cut
            // (reachable from s in the residual graph)
            vector<bool> reachable(n, false);
            dfs_residual(i, reachable, dinic);

            // Update parents for next iterations
            // If any node j > i points to p[i] but is actually on i's side of the cut,
            // we redirect its parent to i.
            for (int j = i + 1; j < n; j++)
            {
                if (reachable[j] && p[j] == p[i])
                {
                    p[j] = i;
                }
            }
        }

        // 3. Precalculate depths to allow simple climbing during queries
        for (int i = 1; i < n; i++)
        {
            depth[i] = depth[p[i]] + 1;
        }
    }

    void dfs_residual(int u, vector<bool> &reachable, Dinic &dinic)
    {
        reachable[u] = true;
        for (auto &e : dinic.adj[u])
        {
            // If capacity remains (cap - flow > 0), it is not saturated,
            // so we can walk across it in the residual graph.
            if (e.cap - e.flow > 0 && !reachable[e.to])
            {
                dfs_residual(e.to, reachable, dinic);
            }
        }
    }

    // Query min cut between any two nodes
    long long min_cut(int u, int v)
    {
        if (u == v)
            return 0; // Or INF depending on problem definition

        long long res = 2e18; // Use large INF

        // Walk up the tree from u and v simultaneously to find their LCA,
        // maintaining the minimum edge weight seen on the path.
        while (u != v)
        {
            if (depth[u] > depth[v])
            {
                res = min(res, weight[u]);
                u = p[u];
            }
            else
            {
                res = min(res, weight[v]);
                v = p[v];
            }
        }
        return res;
    }
};