/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: SEGMENT TREE MERGING
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Subtree queries", "Colors in subtree", "Inversions in subtree", "Tree DP".
 * - Classic Scenarios: You are traversing a tree (or graph) bottom-up. Every node needs
 *   to track a frequency array or sums of its subtree.
 * - The Magic: Merging two Dynamic Segment Trees takes time proportional to their
 *   *intersecting* nodes. If you merge all children into their parents bottom-up,
 *   the total time and memory for the entire process is mathematically bounded to
 *   O(N log(Range)). It feels like magic because $10^5$ independent trees merge into
 *   one in a fraction of a second.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization:
 *       MergeableSegTree st(-1e9, 1e9);
 *       vector<int> roots(N + 1, -1); // Tracks the root ID for each node's personal tree
 *
 * - Updates:
 *       // Add 'val' at coordinate 'idx' for node U's personal tree
 *       roots[U] = st.update(roots[U], idx, val);
 *
 * - Merging:
 *       // Merge the tree of child V into the tree of parent U
 *       roots[U] = st.merge(roots[U], roots[V]);
 *
 * - Queries:
 *       // Query the sum in range [L, R] inside node U's tree
 *       long long ans = st.query(roots[U], L, R);
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - ⚠️ DESTRUCTIVE MERGE WARNING:
 *   This template performs a "Destructive Merge". When you do `st.merge(roots[U], roots[V])`,
 *   the tree of V is absorbed into U. You CANNOT safely query `roots[V]` anymore!
 *   This is perfectly fine for 99% of Tree DP problems (you process bottom-up and don't look back).
 *   If a problem forces you to query V again later, you must change the `merge` function to
 *   create a clone instead of modifying `u` in place (Warning: clones cause massive memory usage).
 * - Changing to Min/Max:
 *   Change `seg[u].sum += seg[v].sum` to `= max(seg[u].max, seg[v].max)` in the base case of `merge()`.
 */

#include <bits/stdc++.h>
using namespace std;

struct MergeableSegTree
{
private:
    struct Node
    {
        long long sum = 0;
        int lc = -1; // Left child index
        int rc = -1; // Right child index
    };

    vector<Node> seg;
    long long MIN_VAL, MAX_VAL;

    int allocate()
    {
        seg.emplace_back();
        return seg.size() - 1;
    }

    int update(int node, long long lx, long long rx, long long idx, long long val)
    {
        // If the node doesn't exist yet, create it!
        if (node == -1)
            node = allocate();

        if (lx == rx)
        {
            seg[node].sum += val;
            return node;
        }

        long long mid = lx + (rx - lx) / 2;
        if (idx <= mid)
        {
            seg[node].lc = update(seg[node].lc, lx, mid, idx, val);
        }
        else
        {
            seg[node].rc = update(seg[node].rc, mid + 1, rx, idx, val);
        }

        // Pull up answers from children
        seg[node].sum = 0;
        if (seg[node].lc != -1)
            seg[node].sum += seg[seg[node].lc].sum;
        if (seg[node].rc != -1)
            seg[node].sum += seg[seg[node].rc].sum;

        return node;
    }

    // ? THE MAGIC: The Destructive Merge Function
    int merge(int u, int v, long long lx, long long rx)
    {
        // 1. If one of the branches is empty, just point to the other! (O(1) shortcut)
        if (u == -1)
            return v;
        if (v == -1)
            return u;

        // 2. Base case: Both trees have data at this leaf. Combine them!
        if (lx == rx)
        {
            seg[u].sum += seg[v].sum;
            return u;
        }

        // 3. Recursive case: Traverse overlapping paths and merge children
        long long mid = lx + (rx - lx) / 2;
        seg[u].lc = merge(seg[u].lc, seg[v].lc, lx, mid);
        seg[u].rc = merge(seg[u].rc, seg[v].rc, mid + 1, rx);

        // Pull up the new combined sum
        seg[u].sum = 0;
        if (seg[u].lc != -1)
            seg[u].sum += seg[seg[u].lc].sum;
        if (seg[u].rc != -1)
            seg[u].sum += seg[seg[u].rc].sum;

        return u; // Return the absorbed parent node
    }

    long long query(int node, long long lx, long long rx, long long ql, long long qr)
    {
        if (node == -1 || ql > rx || qr < lx)
            return 0;

        if (ql <= lx && rx <= qr)
            return seg[node].sum;

        long long mid = lx + (rx - lx) / 2;
        return query(seg[node].lc, lx, mid, ql, qr) +
               query(seg[node].rc, mid + 1, rx, ql, qr);
    }

public:
    MergeableSegTree(long long min_v, long long max_v)
    {
        MIN_VAL = min_v;
        MAX_VAL = max_v;
        // Pre-allocate to prevent slow resizing.
        // Formula: N * log(Range). Usually 40 * N is safe for 10^9 ranges.
        seg.reserve(4000000);
    }

    // Pass the root of the tree you want to update
    int update(int root, long long idx, long long val)
    {
        return update(root, MIN_VAL, MAX_VAL, idx, val);
    }

    // Pass the roots of the two trees you want to merge
    int merge(int root_u, int root_v)
    {
        return merge(root_u, root_v, MIN_VAL, MAX_VAL);
    }

    // Pass the root of the tree you want to query
    long long query(int root, long long l, long long r)
    {
        return query(root, MIN_VAL, MAX_VAL, l, r);
    }
};

void solve()
{
    int n;
    // ... Read tree structure ...
    // vector<vector<int>> adj(n + 1);

    // Create the global Mergeable Segment Tree
    MergeableSegTree st(1, 1e9);

    // Array to track the root of the segment tree belonging to each node
    vector<int> roots(n + 1, -1);

    // Example: Add value '1' to coordinate 'color[u]' for node U's tree
    // roots[u] = st.update(roots[u], color[u], 1);

    // Example Tree DP logic:
    // for (int v : adj[u]) {
    //     dfs(v, u);
    //     roots[u] = st.merge(roots[u], roots[v]);
    // }

    // Now roots[u] contains the fully merged frequency array of its entire subtree!
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    // solve();
    return 0;
}