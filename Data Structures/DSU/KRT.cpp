/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: KRUSKAL RECONSTRUCTION TREE (KRT)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Minimax path", "Reachability under weight limit", "Max edge on path".
 * - Classic Scenarios:
 *   A) You are asked to find a path between U and V that MINIMIZES the MAXIMUM edge weight along the way.
 *   B) You are at city U. You can only use roads with a weight <= W. How many cities can you reach?
 * - The Magic: During Kruskal's MST algorithm, instead of just merging two components, we create a
 *   BRAND NEW artificial node to represent the edge! We make the roots of the two components children
 *   of this new artificial node. The result is a tree where the "leaves" are the original cities, and
 *   the "internal nodes" are the edges! The weight of the ancestors strictly increases as you go up.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Pass the number of nodes (1-indexed) and a vector of Edges.
 *       vector<Edge> edges = {{1, 2, 10}, {2, 3, 20}, {1, 3, 5}};
 *       KRT krt(3, edges);
 *
 * - Minimax Path Query:
 *       // Find the maximum weight edge on the optimal path between U and V
 *       long long max_edge = krt.getMaxEdgeOnPath(U, V);
 *
 * - Reachability Query:
 *       // From U, how many cities can I reach using ONLY edges with weight <= W?
 *       int cities = krt.getReachableLeaves(U, W);
 *
 * - Complexity:
 *       Time: O(E log E) to sort edges and build. O(log N) per query!
 *       Space: O(N log N) memory for Binary Lifting tables.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Max Spanning Tree instead of Min Spanning Tree?
 *   If you want a path that MAXIMIZES the MINIMUM edge, change the `operator<` in the `Edge` struct
 *   to `return w > other.w;`. Then, in `getReachableLeaves`, change the jump condition to
 *   `val[p] >= min_w`.
 * - Disconnected Graphs?
 *   The template natively supports disconnected graphs! `getMaxEdgeOnPath` returns `-1` if U and V
 *   are completely disconnected.
 */

#include <bits/stdc++.h>
using namespace std;

struct Edge
{
    int u, v;
    long long w;
    // Sort ascending for standard Min-MST KRT
    bool operator<(const Edge &other) const
    {
        return w < other.w;
    }
};

struct KRT
{
private:
    int n, krt_nodes;
    const int LOG = 21; // Safe for up to 10^6 nodes

    // DSU parent array (crucial that we don't use size/rank here, we MUST link to the new node)
    vector<int> dsu_parent;

    // KRT structure
    vector<long long> val;    // The weight of the edge this node represents
    vector<vector<int>> adj;  // Tree edges
    vector<int> depth;        // Depth for LCA
    vector<vector<int>> up;   // Binary Lifting table
    vector<int> leaves_count; // How many original cities are in this subtree?

    int find(int i)
    {
        if (i == dsu_parent[i])
            return i;
        // Path compression is perfectly safe here because we only use DSU to find the current root
        return dsu_parent[i] = find(dsu_parent[i]);
    }

    // Standard Binary Lifting DFS
    void dfs(int u, int p)
    {
        up[u][0] = p;
        for (int i = 1; i < LOG; i++)
        {
            up[u][i] = up[up[u][i - 1]][i - 1];
        }

        // Base case: If it has no children, it's an original city (Leaf)
        if (adj[u].empty())
        {
            leaves_count[u] = 1;
        }
        else
        {
            leaves_count[u] = 0;
        }

        for (int v : adj[u])
        {
            if (v != p)
            {
                depth[v] = depth[u] + 1;
                dfs(v, u);
                // Bubble up the leaf count
                leaves_count[u] += leaves_count[v];
            }
        }
    }

public:
    KRT(int n, vector<Edge> &edges)
    {
        this->n = n;
        // The KRT will have exactly N original leaves, and at most N-1 artificial edge-nodes
        int max_nodes = 2 * n + 5;

        dsu_parent.resize(max_nodes);
        iota(dsu_parent.begin(), dsu_parent.end(), 0);

        val.assign(max_nodes, 0);
        adj.resize(max_nodes);
        leaves_count.assign(max_nodes, 0);
        depth.assign(max_nodes, 0);
        up.assign(max_nodes, vector<int>(LOG, 0));

        // 1. Sort the edges to process in Kruskal's order
        sort(edges.begin(), edges.end());

        krt_nodes = n; // Nodes 1 to N are the original cities

        // 2. Build the Tree
        for (auto &e : edges)
        {
            int ru = find(e.u);
            int rv = find(e.v);

            if (ru != rv)
            {
                krt_nodes++; // Create a new artificial node for this edge
                val[krt_nodes] = e.w;

                // Connect the new node to the roots of the two components
                adj[krt_nodes].push_back(ru);
                adj[krt_nodes].push_back(rv);

                // ? THE MAGIC: The new node becomes the ultimate DSU root for both components!
                dsu_parent[ru] = krt_nodes;
                dsu_parent[rv] = krt_nodes;
            }
        }

        // 3. Process the DFS for LCA and Subtree sizes
        // We loop through all nodes in case the graph is a disconnected forest
        for (int i = 1; i <= krt_nodes; i++)
        {
            if (find(i) == i)
            {
                // We found a supreme root! Run DFS downwards.
                dfs(i, 0);
            }
        }
    }

    // Returns the max edge weight on the path between U and V
    long long getMaxEdgeOnPath(int u, int v)
    {
        if (find(u) != find(v))
            return -1; // They are in disconnected components
        if (u == v)
            return 0; // Same node

        // Standard LCA Logic
        if (depth[u] < depth[v])
            swap(u, v);
        int diff = depth[u] - depth[v];

        for (int i = 0; i < LOG; i++)
        {
            if ((diff >> i) & 1)
                u = up[u][i];
        }

        if (u == v)
            return val[u]; // One is an ancestor of the other

        for (int i = LOG - 1; i >= 0; i--)
        {
            if (up[u][i] != up[v][i])
            {
                u = up[u][i];
                v = up[v][i];
            }
        }

        // The LCA node IS the edge that connected them!
        int lca = up[u][0];
        return val[lca];
    }

    // Returns how many original cities can be reached from U using edges <= max_w
    int getReachableLeaves(int u, long long max_w)
    {
        // Jump up the tree as long as the parent edge weight is valid
        for (int i = LOG - 1; i >= 0; i--)
        {
            int p = up[u][i];
            // Parent 0 means we hit the root boundaries.
            if (p != 0 && val[p] <= max_w)
            {
                u = p; // Safe to jump!
            }
        }
        // Once we reach the highest valid ancestor, its subtree contains ALL reachable cities!
        return leaves_count[u];
    }
};

void solve()
{
    int n, m, q;
    // Read N nodes, M edges, Q queries
    if (!(cin >> n >> m >> q))
        return;

    vector<Edge> edges(m);
    for (int i = 0; i < m; i++)
    {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }

    KRT krt(n, edges);

    while (q--)
    {
        int type;
        cin >> type;
        if (type == 1)
        {
            int u, v;
            cin >> u >> v;
            long long ans = krt.getMaxEdgeOnPath(u, v);
            if (ans == -1)
                cout << "Disconnected\n";
            else
                cout << ans << "\n";
        }
        else
        {
            int u;
            long long w;
            cin >> u >> w;
            cout << krt.getReachableLeaves(u, w) << "\n";
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    // solve();
    return 0;
}