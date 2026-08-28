/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: 1D FENWICK TREE (RANGE UPDATE & QUERY)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Range addition", "Subarray sum", "Add X to range [L, R]".
 * - Classic Scenarios: You are given an array. You need to repeatedly add a value V
 *   to all elements from index L to R. Then, you need to query the sum of all elements
 *   from index L to R.
 * - The Magic: Normally, you need a Lazy Segment Tree for this. However, by using
 *   two parallel Fenwick Trees, we can track the difference array and its index-multiplier
 *   to mathematically simulate Lazy Propagation. It is ridiculously fast and takes zero
 *   extra memory.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Pass the size of the array to the constructor.
 *   (Note: Fenwick Trees MUST be 1-indexed. The bounds are [1, N]).
 *       FenwickTreeRange fenwick(N);
 *
 * - Range Updates (Inclusive 1-indexed [L, R]):
 *       // Add 'val' to every cell from index L to R
 *       fenwick.update(L, R, val);
 *
 * - Range Queries (Inclusive 1-indexed [L, R]):
 *       // Get the sum of all cells from index L to R
 *       long long total = fenwick.query(L, R);
 *
 * - Complexity:
 *       Time: O(log N) per update and query.
 *       Space: O(N) memory.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Initial Array Values?
 *   If the problem gives you an initial array `A`, you don't need a special build function.
 *   Simply loop through the array and call `fenwick.update(i, i, A[i]);` for each element.
 * - 0-Indexed Input?
 *   If the problem uses 0-based indices, add +1 to L and R before passing them to
 *   `update` or `query`. NEVER change the internal 1-based logic, as `idx & -idx` breaks on 0.
 * - Modulo Arithmetic?
 *   If the problem asks for the sum modulo 10^9+7, wrap the `+=` and `-=` assignments inside
 *   `add` and `queryPrefix` with modulo logic: `res = (res % MOD + MOD) % MOD;`.
 */

#include <bits/stdc++.h>
using namespace std;

struct FenwickTreeRange
{
private:
    int n;

    // bit1 tracks the standard difference array: D[i]
    // bit2 tracks the difference array multiplied by the index: D[i] * i
    vector<long long> bit1, bit2;

    // Point add to both parallel BITs
    void add(int idx, long long val)
    {
        // We use the actual `idx` where the value was inserted for the bit2 multiplier
        for (int i = idx; i <= n; i += i & -i)
        {
            bit1[i] += val;
            bit2[i] += val * idx;
        }
    }

    // Get the sum from index 1 to idx
    long long queryPrefix(int idx)
    {
        long long res = 0;
        for (int i = idx; i > 0; i -= i & -i)
        {
            // The magic algebraic expansion: Sum = (idx + 1) * D[i] - (D[i] * i)
            res += bit1[i] * (idx + 1) - bit2[i];
        }
        return res;
    }

public:
    FenwickTreeRange(int size)
    {
        n = size;
        bit1.assign(n + 1, 0);
        bit2.assign(n + 1, 0);
    }

    // Add `val` to the inclusive range [l, r]
    void update(int l, int r, long long val)
    {
        // Standard difference array inclusion-exclusion logic
        add(l, val);
        add(r + 1, -val);
    }

    // Get the sum of the inclusive range [l, r]
    long long query(int l, int r)
    {
        return queryPrefix(r) - queryPrefix(l - 1);
    }
};

void solve()
{
    int n, q;
    // Example Input Format: N elements, Q queries
    if (!(cin >> n >> q))
        return;

    FenwickTreeRange fenwick(n);

    while (q--)
    {
        int type;
        cin >> type;

        if (type == 1)
        { // Range Add
            int l, r;
            long long val;
            cin >> l >> r >> val;
            fenwick.update(l, r, val);
        }
        else if (type == 2)
        { // Range Query
            int l, r;
            cin >> l >> r;
            cout << fenwick.query(l, r) << "\n";
        }
    }
}

int main()
{
    // Fast I/O for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}