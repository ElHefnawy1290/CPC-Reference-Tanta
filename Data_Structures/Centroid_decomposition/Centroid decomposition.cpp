/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: CENTROID DECOMPOSITION
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Paths of length K", "Distance between all pairs", "Update node color, find closest red node".
 * - Classic Scenarios: You need to answer heavy path queries on a massive tree where standard DFS
 *   would Time Limit Exceed (TLE).
 * - The Magic: We find the "center of gravity" (Centroid) of the tree, make it a root, and "remove" it.
 *   This shatters the graph into a forest. We repeat this recursively. The resulting "Centroid Tree"
 *   connects these centers and mathematically guarantees a maximum depth of O(log N).
 *   You can safely brute-force climbing from any node to the root of the Centroid Tree in just ~17 steps!
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Pass the size of the tree (1-indexed) and the adjacency list.
 *       CentroidDecomposition cd(N, adj);
 *
 * - Navigation: The `cd.centroid_parent[u]` array tells you the parent of node `u` in the new tree.
 * - Distance: Use `cd.get_dist(u, v)` to find the distance between nodes in the ORIGINAL tree.
 *
 * - Complexity:
 *       Time: O(N log N) to build the tree and the LCA tables.
 *       Space: O(N log N) memory for binary lifting tables.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - How to solve a problem with it:
 *   Instead of answering queries globally, answer them for "all paths passing through the current centroid".
 *   For point updates (e.g., painting a node red), start at the updated node and climb up its
 *   `centroid_parent` path, updating the answer at each ancestor using `get_dist(u, ancestor)`.
 */

#include <bits/stdc++.h>
using namespace std;

struct CentroidDecomposition
{
    int n;
    vector<vector<int>> adj;

    // Variables for the Centroid Tree
    vector<int> sz;
    vector<bool> removed;
    vector<int> centroid_parent;

    // Variables for O(1) distance queries in the original tree (Binary Lifting)
    const int LOG = 20;
    vector<vector<int>> up;
    vector<int> depth;

    CentroidDecomposition(int nodes, const vector<vector<int>> &graph)
    {
        n = nodes;
        adj = graph;

        sz.assign(n + 1, 0);
        removed.assign(n + 1, false);
        centroid_parent.assign(n + 1, -1);

        up.assign(n + 1, vector<int>(LOG, 0));
        depth.assign(n + 1, 0);

        // 1. Precompute LCA for fast distance queries
        lca_dfs(1, 1);

        // 2. Build the Centroid Tree
        decompose(1, -1);
    }

    // Standard Binary Lifting DFS to compute depths and ancestors
    void lca_dfs(int u, int p)
    {
        up[u][0] = p;
        for (int i = 1; i < LOG; i++)
        {
            up[u][i] = up[up[u][i - 1]][i - 1];
        }
        for (int v : adj[u])
        {
            if (v != p)
            {
                depth[v] = depth[u] + 1;
                lca_dfs(v, u);
            }
        }
    }

    int get_lca(int u, int v)
    {
        if (depth[u] < depth[v])
            swap(u, v);
        int diff = depth[u] - depth[v];
        for (int i = 0; i < LOG; i++)
        {
            if ((diff >> i) & 1)
                u = up[u][i];
        }
        if (u == v)
            return u;
        for (int i = LOG - 1; i >= 0; i--)
        {
            if (up[u][i] != up[v][i])
            {
                u = up[u][i];
                v = up[v][i];
            }
        }
        return up[u][0];
    }

    // Get the shortest path distance between u and v in the ORIGINAL tree
    int get_dist(int u, int v)
    {
        return depth[u] + depth[v] - 2 * depth[get_lca(u, v)];
    }

    // Your exact dfs function, slightly modified to ignore "removed" nodes!
    void get_sz(int u, int p)
    {
        sz[u] = 1;
        for (int v : adj[u])
        {
            if (v == p || removed[v])
                continue; // Ignore dead ends
            get_sz(v, u);
            sz[u] += sz[v];
        }
    }

    // Your exact centroid function, slightly modified to ignore "removed" nodes!
    int get_centroid(int u, int p, int total_nodes)
    {
        for (int v : adj[u])
        {
            if (v == p || removed[v])
                continue; // Ignore dead ends

            if (sz[v] > total_nodes / 2)
            {
                return get_centroid(v, u, total_nodes);
            }
        }
        return u;
    }

    // The master function that shatters the graph and builds the new tree
    void decompose(int u, int p)
    {
        // 1. Calculate the subtree sizes of the current shattered component
        get_sz(u, -1);
        int total_nodes = sz[u];

        // 2. Find the exact center of gravity (Centroid) for this component
        int c = get_centroid(u, -1, total_nodes);

        // 3. Link it to the Centroid Tree!
        centroid_parent[c] = p;

        // 4. "Delete" this centroid to shatter the tree into smaller forests
        removed[c] = true;

        // 5. Recursively find the centroids of the remaining pieces
        for (int v : adj[c])
        {
            if (!removed[v])
            {
                decompose(v, c);
            }
        }
    }
};

void solve()
{
    int n, q;
    // Read N nodes and Q queries
    if (!(cin >> n >> q))
        return;

    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; i++)
    {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Build the magical O(log N) depth tree
    CentroidDecomposition cd(n, adj);

    // EXAMPLE QUERY: "Update node U's color, then find closest node with that color"
    // To solve this, you would start at U and literally climb the centroid tree!
    /*
    int u = 5;
    int curr = u;
    while (curr != -1) {
        int distance_in_original_tree = cd.get_dist(u, curr);
        // update_answer(curr, distance_in_original_tree);
        curr = cd.centroid_parent[curr];
    }
    */
}

int main()
{
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}