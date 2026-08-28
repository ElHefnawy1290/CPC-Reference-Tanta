/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: ITERATIVE SEGMENT TREE
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Point Updates", "Range Queries", "Strict Time Limits", "No Lazy Propagation".
 * - Classic Scenarios: You need a Segment Tree, but the time limit is extremely tight (e.g., 1.0s for 5*10^5 queries),
 *   or the memory limit is small. Recursive segment trees have overhead from function calls and use 4*N memory.
 *   This iterative version uses EXACTLY 2*N memory and runs in a tight `while` loop, making it blindingly fast.
 * - The Magic: The leaves of the tree are stored in indices `N` to `2N-1`. The parents are stored in `1` to `N-1`.
 *   To find the parent of node `x`, you just do `x / 2` (or `x >> 1`).
 *   To find the sibling of node `x`, you just do `x ^ 1`.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Pass a 0-indexed vector to build the tree.
 *       vector<long long> a = {1, 5, 2, 8, 3};
 *       IterativeSegTree st(a);
 *
 * - Updates (0-indexed point SET):
 *       // Replace the value at index `idx` with `X`
 *       st.update(idx, X);
 *
 * - Queries (0-indexed, Inclusive [L, R]):
 *       // Query the combined answer from index L to R
 *       long long ans = st.query(L, R);
 *
 * - Complexity:
 *       Time: O(N) to build. O(log N) per update and query (with a tiny constant factor).
 *       Space: O(N) memory. Uses exactly 2*N elements.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Changing from Range Sum to Min/Max/GCD?
 *   1. Change the `NEUTRAL` variable to `1e18` (Min), `-1e18` (Max), or `0` (GCD).
 *   2. Update the `combine()` function to return `min(a, b)`, `max(a, b)`, or `gcd(a, b)`.
 * - Changing `update` from "Set" to "Add"?
 *   In `update()`, change `seg[idx += n] = val;` to `seg[idx += n] += val;`.
 */

#include <bits/stdc++.h>
using namespace std;

struct IterativeSegTree
{
private:
    int n;
    vector<long long> seg;

    // ? Dial 1: The Neutral Element (Identity)
    // 0 for Sum, 1e18 for Min, -1e18 for Max, 0 for GCD
    const long long NEUTRAL = 0;

    // ? Dial 2: The Combination Logic
    long long combine(long long a, long long b)
    {
        return a + b;
    }

public:
    IterativeSegTree(const vector<long long> &v)
    {
        n = v.size();
        // Allocate exactly 2 * N memory. The 0-th index is unused dummy space.
        seg.assign(2 * n, NEUTRAL);

        // 1. Insert the leaf nodes directly into the second half of the array
        for (int i = 0; i < n; i++)
        {
            seg[n + i] = v[i];
        }

        // 2. Build the parents from bottom to top
        for (int i = n - 1; i > 0; i--)
        {
            // A node `i` has children at `2*i` and `2*i + 1`
            seg[i] = combine(seg[i << 1], seg[i << 1 | 1]);
        }
    }

    // Replace the value at 0-indexed position `idx` with `val`
    void update(int idx, long long val)
    {
        // Jump straight to the leaf node
        idx += n;
        seg[idx] = val; // ! Change to += if problem requires Point Add instead of Point Set

        // Climb to the root, updating parents
        // `idx > 1` stops us before we update the unused 0-th index
        for (idx >>= 1; idx > 0; idx >>= 1)
        {
            // `idx << 1` is left child, `idx << 1 | 1` is right child
            seg[idx] = combine(seg[idx << 1], seg[idx << 1 | 1]);
        }
    }

    // Query the inclusive range [l, r]
    long long query(int l, int r)
    {
        long long res_left = NEUTRAL;
        long long res_right = NEUTRAL;

        // Jump straight to the leaves
        // Note: We use inclusive [l, r] so our loop condition is l <= r
        for (l += n, r += n; l <= r; l >>= 1, r >>= 1)
        {

            // ? If `l` is odd, it is a right child.
            // This means its parent covers data OUTSIDE our [l, r] range!
            // We must process this node independently, then move `l` to the right.
            if (l & 1)
            {
                res_left = combine(res_left, seg[l++]);
            }

            // ? If `r` is even, it is a left child.
            // This means its parent covers data OUTSIDE our [l, r] range!
            // We must process this node independently, then move `r` to the left.
            if (!(r & 1))
            {
                res_right = combine(seg[r--], res_right);
            }
        }

        // Combine the results from the left sweep and the right sweep.
        // Doing it this way preserves the strict Left-to-Right order for non-commutative operations!
        return combine(res_left, res_right);
    }
};

void solve()
{
    int n, q;
    if (!(cin >> n >> q))
        return;

    vector<long long> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];

    IterativeSegTree st(a);

    while (q--)
    {
        int type;
        cin >> type;
        if (type == 1)
        {
            int idx;
            long long val;
            cin >> idx >> val;
            st.update(idx, val); // 0-indexed update
        }
        else
        {
            int l, r;
            cin >> l >> r;
            cout << st.query(l, r) << "\n"; // 0-indexed query
        }
    }
}

int main()
{
    // Standard fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}