/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: PERSISTENT LAZY SEGMENT TREE
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Time travel", "Historical versions", "Range updates on previous states".
 * - Classic Scenarios: You are given an array. You need to perform Range Additions or Range Sets.
 *   Each update creates a new "version" of the array. You must be able to query the Range Sum
 *   of ANY past version of the array efficiently.
 * - The Magic: Instead of modifying nodes in place, we create a clone of every node we touch.
 *   This means `update(v, L, R)` leaves version `v` completely untouched and returns the root ID
 *   of the brand new version. Because we only change O(log N) nodes per update, memory and time
 *   remain highly efficient!
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Pass a 0-indexed vector<long long> to build Version 0.
 *       vector<long long> a = {1, 5, 2, 8, 3};
 *       PersistentLazySegTree pst(a);
 *
 * - Tracking Versions: Every update returns a NEW root index. You should store these!
 *       vector<int> versions;
 *       versions.push_back(pst.roots[0]); // Initial version
 *
 * - Updates (All 0-indexed, Inclusive [L, R]):
 *       // Add X to range [L, R] on a specific version, and save the new version ID
 *       int new_root = pst.updateAdd(versions.back(), L, R, X);
 *       versions.push_back(new_root);
 *
 * - Queries (All 0-indexed, Inclusive [L, R]):
 *       // Query the sum of range [L, R] exactly as it was in version 'V'
 *       long long sum = pst.querySum(versions[V], L, R);
 *
 * - Complexity:
 *       Time: O(N) to build. O(log N) per update and query.
 *       Space: O(N + Q log N) memory.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Memory Limit Exceeded (MLE)? Persistent Lazy creates ~4 new nodes per update.
 *   If N = 10^5 and Q = 10^5, you will generate roughly 10^5 + 400,000 nodes.
 *   The constructor already calls `seg.reserve(N * 40)` to prevent expensive vector reallocations.
 *   Increase this multiplier if Q is exceptionally large.
 * - Changing from Range Add to Range Set?
 *   Range Add is special because we can just accumulate `lazy` tags during the query without
 *   ever modifying the tree. If you want Range Set, you must pass down a `lazySet` flag.
 *   In `query`, if you hit a node with an active `lazySet`, simply return `lazySet * (r - l + 1)`
 *   immediately, ignoring its children.
 */

#include <bits/stdc++.h>
using namespace std;

struct PersistentLazySegTree
{
private:
    struct Node
    {
        int lc, rc;     // Indices of the left and right children
        long long sum;  // The range sum
        long long lazy; // The lazy tag (Range Add)

        Node() : lc(0), rc(0), sum(0), lazy(0) {}
        Node(int lc, int rc, long long sum, long long lazy)
            : lc(lc), rc(rc), sum(sum), lazy(lazy) {}
    };

    int n;
    vector<Node> seg;

    // ? Creates a brand new node that is an exact copy of the given node ID
    int clone(int node)
    {
        seg.push_back(seg[node]);
        return seg.size() - 1;
    }

    int build(int l, int r, const vector<long long> &a)
    {
        int curr = seg.size();
        seg.emplace_back(); // Allocate new node

        if (l == r)
        {
            if (l < (int)a.size())
            {
                seg[curr].sum = a[l];
            }
            return curr;
        }

        int mid = l + (r - l) / 2;
        int left_child = build(l, mid, a);
        int right_child = build(mid + 1, r, a);

        seg[curr].lc = left_child;
        seg[curr].rc = right_child;
        seg[curr].sum = seg[left_child].sum + seg[right_child].sum;

        return curr;
    }

    int updateAdd(int node, int l, int r, int ql, int qr, long long val)
    {
        // ? Core Persistence Logic: NEVER modify the original node. Always work on a clone!
        int curr = clone(node);

        // 1. Total Overlap
        if (ql <= l && r <= qr)
        {
            seg[curr].sum += val * (r - l + 1);
            seg[curr].lazy += val;
            return curr;
        }

        int mid = l + (r - l) / 2;

        // ? Lazy Propagation in Persistence:
        // Before we branch down, we must push the current node's lazy tag to its children.
        // Because pushing modifies the children, we MUST clone the children first!
        if (seg[curr].lazy != 0)
        {
            if (l != r)
            { // Don't push down if it's a leaf
                int left_child = clone(seg[curr].lc);
                seg[left_child].sum += seg[curr].lazy * (mid - l + 1);
                seg[left_child].lazy += seg[curr].lazy;
                seg[curr].lc = left_child;

                int right_child = clone(seg[curr].rc);
                seg[right_child].sum += seg[curr].lazy * (r - mid);
                seg[right_child].lazy += seg[curr].lazy;
                seg[curr].rc = right_child;
            }
            seg[curr].lazy = 0; // Cleared from the current node
        }

        // 2. Partial Overlap (Recursion)
        if (ql <= mid)
        {
            seg[curr].lc = updateAdd(seg[curr].lc, l, mid, ql, qr, val);
        }
        if (qr > mid)
        {
            seg[curr].rc = updateAdd(seg[curr].rc, mid + 1, r, ql, qr, val);
        }

        // 3. Update Current Node Sum
        seg[curr].sum = seg[seg[curr].lc].sum + seg[seg[curr].rc].sum;
        return curr;
    }

    // ? IMPORTANT: We CANNOT push down lazy tags during a query. Pushing clones nodes,
    // which would cause a read-only query to consume massive amounts of memory!
    // Instead, we just pass the accumulated lazy value down the recursion.
    long long querySum(int node, int l, int r, int ql, int qr, long long lazy_acc)
    {
        if (!node || ql > r || qr < l)
            return 0;

        // 1. Total Overlap
        if (ql <= l && r <= qr)
        {
            // Apply all the pending lazy additions from ancestors to this block
            return seg[node].sum + lazy_acc * (r - l + 1);
        }

        int mid = l + (r - l) / 2;

        // 2. Add the CURRENT node's lazy tag to the accumulator for its children
        long long current_lazy = lazy_acc + seg[node].lazy;

        // 3. Partial Overlap
        long long res = 0;
        if (ql <= mid)
        {
            res += querySum(seg[node].lc, l, mid, ql, qr, current_lazy);
        }
        if (qr > mid)
        {
            res += querySum(seg[node].rc, mid + 1, r, ql, qr, current_lazy);
        }

        return res;
    }

public:
    // Keeps track of the root node index for every version
    vector<int> roots;

    PersistentLazySegTree(const vector<long long> &a)
    {
        n = a.size();

        // Pre-allocate memory to prevent expensive vector resizing.
        // Formula: N + (Queries * 4 * log2(N)). 40 is a safe constant for 10^5.
        seg.reserve(n * 40);

        // Create a dummy node 0 (Null Node)
        seg.emplace_back();

        // Build Version 0
        roots.push_back(build(0, n - 1, a));
    }

    // Pass the root ID of the version you want to update, get the new root ID back
    int updateAdd(int root_version, int l, int r, long long val)
    {
        return updateAdd(root_version, 0, n - 1, l, r, val);
    }

    // Pass the root ID of the version you want to query
    long long querySum(int root_version, int l, int r)
    {
        return querySum(root_version, 0, n - 1, l, r, 0LL);
    }
};