#include <bits/stdc++.h>
using namespace std;

/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: VIRTUAL TREE (AUXILIARY TREE)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Paths between subset nodes", "DP on subset of tree", "Sparse subset queries".
 * - Classic Scenarios: A tree has N=10^5 nodes. You have Q queries. Each query gives you a subset
 *   of K nodes. You need to perform a Tree DP or path calculation on these K nodes, but running
 *   an O(N) DFS per query is impossible.
 * - The Magic: The Virtual Tree builds a smaller tree containing only the K subset nodes
 *   AND their necessary LCAs to maintain tree structure. This new tree has O(K) nodes.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization:
 *       1. Build the full tree using standard DFS.
 *       2. For each query, collect the subset of K nodes.
 *       3. Call `build_virtual_tree(subset)`.
 *       4. Perform your DP on `virtual_adj` (the edges of the virtual tree).
 * - Complexity:
 *       Time: O(K log K) per query to build.
 *       Space: O(N) to store the full tree + O(K) for the virtual tree.
 */

const int MAXN = 2e5 + 5;
const int LOG = 20;

// adj: Original tree, virtual_adj: The sparse tree we build for the subset
vector<int> adj[MAXN], virtual_adj[MAXN];
int tin[MAXN], tout[MAXN], depth[MAXN];
int up[MAXN][LOG];
int timer;

// Standard DFS to precalculate depths, entry times, and binary lifting table
void dfs(int u, int p)
{
    tin[u] = ++timer;
    depth[u] = depth[p] + 1;
    up[u][0] = p;
    for (int i = 1; i < LOG; i++)
        up[u][i] = up[up[u][i - 1]][i - 1];

    for (int v : adj[u])
    {
        if (v != p)
            dfs(v, u);
    }
    tout[u] = timer;
}

// Find LCA using binary lifting
int get_lca(int u, int v)
{
    if (depth[u] < depth[v])
        swap(u, v);
    for (int i = LOG - 1; i >= 0; i--)
    {
        if (depth[u] - (1 << i) >= depth[v])
            u = up[u][i];
    }
    if (u == v)
        return u;
    for (int i = LOG - 1; i >= 0; i--)
    {
        if (up[u][i] != up[v][i])
        {
            u = up[u][i], v = up[v][i];
        }
    }
    return up[u][0];
}

// Ancestor check using Entry/Exit times
bool is_ancestor(int u, int v)
{
    return tin[u] <= tin[v] && tout[u] >= tout[v];
}

// Main function to build the virtual tree from a subset of nodes
void build_virtual_tree(vector<int> &nodes)
{
    // 1. Sort by entry time so we can process the tree in DFS order
    sort(nodes.begin(), nodes.end(), [](int a, int b)
         { return tin[a] < tin[b]; });

    // 2. Inject LCAs: To keep the tree connected, we must add the LCA of every
    // adjacent pair in our sorted list.
    int sz = nodes.size();
    for (int i = 0; i < sz - 1; i++)
    {
        nodes.push_back(get_lca(nodes[i], nodes[i + 1]));
    }

    // 3. Sort again by entry time and remove duplicates (LCAs might have been added multiple times)
    sort(nodes.begin(), nodes.end(), [](int a, int b)
         { return tin[a] < tin[b]; });
    nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());

    // 4. Connect the nodes using a Stack
    // We maintain a stack representing the current path from the root.
    vector<int> st;
    st.push_back(nodes[0]);

    for (size_t i = 1; i < nodes.size(); i++)
    {
        // Pop nodes that are not ancestors of the current node
        while (st.size() > 1 && !is_ancestor(st.back(), nodes[i]))
        {
            virtual_adj[st[st.size() - 2]].push_back(st.back());
            st.pop_back();
        }
        st.push_back(nodes[i]);
    }
    // Connect remaining nodes in the stack
    while (st.size() > 1)
    {
        virtual_adj[st[st.size() - 2]].push_back(st.back());
        st.pop_back();
    }
}

void solve()
{
    int n;
    cin >> n;

    // Clear graph before building for multiple test cases
    for (int i = 0; i <= n; ++i)
    {
        adj[i].clear();
        virtual_adj[i].clear();
    }

    // Example: Reading N-1 edges
    for (int i = 0; i < n - 1; ++i)
    {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Precompute full tree properties
    timer = 0;
    dfs(1, 1);

    int k;
    cin >> k;
    vector<int> subset(k);
    for (int &x : subset)
        cin >> x;

    // Build the virtual tree for the K nodes
    build_virtual_tree(subset);

    // Now perform DP on `virtual_adj`!
    // subset now contains all nodes in the virtual tree.
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}