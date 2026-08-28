/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: OLD DRIVER TREE (ODT / CHTHOLLY TREE)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Color blocks", "Range assign", "Randomized data", "Interval set".
 * - Classic Scenarios: You have an array that is mostly composed of long segments of identical
 *   values. You need to perform Range Assignments (A[i] = V for L to R) and other Range Queries.
 * - The Magic: An ODT stores the array as a set of intervals {l, r, v}.
 *   The "Assign" operation is the key: it erases all intervals in the set between [L, R]
 *   and inserts one single new interval. This keeps the number of intervals very small.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Insert the initial [1, N] interval into the `odt` set.
 * - Split: You MUST call `split(l)` and `split(r+1)` before ANY operation to ensure
 *   the boundaries are exact.
 * - Complexity:
 *       Time: Amortized O(N log log N) on random data. O(N^2) on worst-case non-random data.
 *       Space: O(N) memory.
 */

#include <bits/stdc++.h>
using namespace std;

struct Node
{
    int l, r;
    mutable long long v;

    Node(int l, int r = -1, long long v = 0) : l(l), r(r), v(v) {}

    // Set must be sorted by left endpoint
    bool operator<(const Node &o) const
    {
        return l < o.l;
    }
};

set<Node> odt;

// Splits the interval containing 'pos' into [l, pos-1] and [pos, r]
// Returns an iterator to the interval starting at 'pos'
auto split(int pos)
{
    auto it = odt.lower_bound({pos});
    if (it != odt.end() && it->l == pos)
        return it;

    --it;
    int l = it->l, r = it->r;
    long long v = it->v;

    odt.erase(it);
    odt.insert({l, pos - 1, v});
    return odt.insert({pos, r, v}).first;
}

// ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
// - If the problem asks for Range Sum: loop through the set from itl to itr and sum (r-l+1)*v.
// - If the problem asks for K-th smallest or Count: you must keep a secondary data
//   structure (like a Segment Tree) updated alongside the ODT.
// The core efficiency of ODT: replaces range [l, r] with one single interval
void assign(int l, int r, long long v)
{
    auto itr = split(r + 1), itl = split(l);
    odt.erase(itl, itr);
    odt.insert({l, r, v});
}

void solve()
{
    int n, m;
    cin >> n >> m;

    // Initially add [1, n] with value 0
    odt.insert({1, n, 0});
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}