/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: DYNAMIC SEGMENT TREE
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Massive constraints", "10^9 or 10^18 indices", "Sparse updates", "Avoid Coordinate Compression".
 * - Classic Scenarios: You need a Segment Tree over a massive range of coordinates (e.g., from -10^9 to 10^9).
 *   A standard segment tree would need an array of size 4 * 10^9, which guarantees a Memory Limit Exceeded (MLE) crash.
 * - The Magic: Instead of creating all nodes at the start, we only create nodes *as we visit them*.
 *   Initially, only the Root exists. When an update goes down the tree, it allocates the exact $O(\log(\text{Range}))$
 *   children it needs to reach the bottom. Empty space simply doesn't exist in memory!
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Pass the absolute minimum and maximum possible coordinates to the constructor.
 *       // Create a tree covering the range [-10^9, 10^9]
 *       DynamicSegTree st(-1e9, 1e9);
 *
 * - Updates (0-indexed point update):
 *       // Add X to the value at coordinate `idx`
 *       st.update(idx, X);
 *
 * - Queries (Inclusive [L, R]):
 *       // Query the sum of elements from coordinate L to R
 *       long long total = st.query(L, R);
 *
 * - Complexity:
 *       Time: $O(\log(\text{MAX\_VAL} - \text{MIN\_VAL}))$ per update and query.
 *       Space: $O(Q \log(\text{MAX\_VAL} - \text{MIN\_VAL}))$. (Creates ~30 nodes per update for 10^9 ranges).
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Changing to Range Min/Max?
 *   1. Change `Node::sum` to `val` and initialize to `1e18` (Min) or `-1e18` (Max).
 *   2. Change the `NEUTRAL` variable to `1e18` or `-1e18`.
 *   3. In `query()`, return `min/max` instead of `+`.
 * - "My code crashes randomly during updates!":
 *   Never use references (`Node& curr = seg[node]`) before calling `.push_back()`. Vectors reallocate memory when they
 *   grow, which instantly invalidates all references to its elements, causing a terrifying segmentation fault.
 *   This template strictly uses indices (`seg[node]`) to completely avoid this trap.
 */

#include <bits/stdc++.h>
using namespace std;

struct DynamicSegTree
{
private:
    struct Node
    {
        long long sum;
        int lc; // Index of Left Child in the vector
        int rc; // Index of Right Child in the vector

        Node() : sum(0), lc(-1), rc(-1) {}
    };

    vector<Node> seg;
    long long MIN_VAL, MAX_VAL;
    const long long NEUTRAL = 0; // 0 for Sum. 1e18 for Min. -1e18 for Max.

    // ? Creates children on-demand if they don't exist yet
    void extend(int node)
    {
        if (seg[node].lc == -1)
        {
            seg[node].lc = seg.size();
            seg.emplace_back(); // Safely allocates a new blank node
        }
        if (seg[node].rc == -1)
        {
            seg[node].rc = seg.size();
            seg.emplace_back(); // Safely allocates a new blank node
        }
    }

    void update(int node, long long lx, long long rx, long long idx, long long val)
    {
        if (lx == rx)
        {
            seg[node].sum += val; // Point addition
            return;
        }

        // ! We are going deeper, so we MUST ensure the children exist
        extend(node);

        // ? This safe midpoint calculation gracefully handles negative ranges
        long long mid = lx + (rx - lx) / 2;

        if (idx <= mid)
        {
            update(seg[node].lc, lx, mid, idx, val);
        }
        else
        {
            update(seg[node].rc, mid + 1, rx, idx, val);
        }

        // Pull up the answer from the children
        seg[node].sum = seg[seg[node].lc].sum + seg[seg[node].rc].sum;
    }

    long long query(int node, long long lx, long long rx, long long ql, long long qr)
    {
        // ! If this branch doesn't exist, it means nothing was ever added here! Return Neutral.
        if (node == -1 || ql > rx || qr < lx)
        {
            return NEUTRAL;
        }

        // Total overlap
        if (ql <= lx && rx <= qr)
        {
            return seg[node].sum;
        }

        long long mid = lx + (rx - lx) / 2;

        // Partial overlap
        long long left_ans = query(seg[node].lc, lx, mid, ql, qr);
        long long right_ans = query(seg[node].rc, mid + 1, rx, ql, qr);

        return left_ans + right_ans;
    }

public:
    DynamicSegTree(long long min_v, long long max_v)
    {
        MIN_VAL = min_v;
        MAX_VAL = max_v;

        // Pre-allocate some memory to minimize slow vector reallocations
        seg.reserve(1000000);

        // Create the root node at index 0
        seg.emplace_back();
    }

    // Call this to add 'val' to coordinate 'idx'
    void update(long long idx, long long val)
    {
        update(0, MIN_VAL, MAX_VAL, idx, val);
    }

    // Call this to get the sum of the range [l, r]
    long long query(long long l, long long r)
    {
        return query(0, MIN_VAL, MAX_VAL, l, r);
    }
};