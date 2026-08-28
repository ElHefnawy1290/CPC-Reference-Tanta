/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: 2D FENWICK TREE (RANGE UPDATE & QUERY)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "2D Grid", "Add X to subgrid", "Sum of subgrid", "Rectangle Updates".
 * - Classic Scenarios: You are given an N x M grid (like a terrain map or matrix).
 *   You need to repeatedly add a value V to all cells in a rectangle from (X1, Y1) to (X2, Y2).
 *   Then, you need to query the sum of all cells in a rectangle from (X1, Y1) to (X2, Y2).
 * - The Magic: A 2D Segment tree with Lazy Propagation is notoriously difficult to code and uses
 *   gigantic amounts of memory. This 2D Fenwick Tree uses 4 parallel grids to mathematically
 *   track the inclusion-exclusion bounds of the updates. It uses a fraction of the memory
 *   and runs blazingly fast in O(log N * log M) time.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Pass the dimensions (rows, cols) to the constructor.
 *   (Note: Fenwick Trees MUST be 1-indexed. The bounds are [1, N] and [1, M]).
 *       FenwickTree2DRange fenwick(N, M);
 *
 * - Range Updates (Inclusive 1-indexed [x1, x2] and [y1, y2]):
 *       // Add 'val' to every cell in the rectangle bounded by top-left (x1, y1) and bottom-right (x2, y2)
 *       fenwick.update(x1, y1, x2, y2, val);
 *
 * - Range Queries (Inclusive 1-indexed [x1, x2] and [y1, y2]):
 *       // Get the sum of all cells in the rectangle
 *       long long total = fenwick.query(x1, y1, x2, y2);
 *
 * - Complexity:
 *       Time: O(log N * log M) per update and query.
 *       Space: O(N * M) memory.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - 0-Indexed Input?
 *   If your problem gives coordinates as 0-indexed (e.g., (0, 0) to (N-1, M-1)), simply
 *   add +1 to all coordinates before passing them into `update` and `query`.
 *   NEVER change the internal 1-based indexing of the BIT, as `index & -index` fails on 0!
 * - Modulo Arithmetic?
 *   If the problem asks for the answer modulo 10^9+7, wrap every `+=` and `-=` inside the
 *   `add` and `queryPrefix` functions with modulo logic: `res = (res % MOD + MOD) % MOD;`
 */

#include <bits/stdc++.h>
using namespace std;

struct FenwickTree2DRange
{
private:
    int n, m;

    // We need 4 parallel 2D BITs to mathematically calculate the 2D Prefix Sums
    // D[i][j], D[i][j] * i, D[i][j] * j, D[i][j] * i * j
    vector<vector<long long>> bit1, bit2, bit3, bit4;

    // Add value to coordinate (x, y) across all 4 mathematical states
    void add(int x, int y, long long val)
    {
        for (int i = x; i <= n; i += i & -i)
        {
            for (int j = y; j <= m; j += j & -j)
            {
                bit1[i][j] += val;
                bit2[i][j] += val * x;
                bit3[i][j] += val * y;
                bit4[i][j] += val * x * y;
            }
        }
    }

    // Get the sum of the rectangle from (1, 1) to (x, y)
    long long queryPrefix(int x, int y)
    {
        long long res = 0;
        for (int i = x; i > 0; i -= i & -i)
        {
            for (int j = y; j > 0; j -= j & -j)
            {
                // The mathematical expansion of 2D Difference Array Prefix Sums:
                // Sum = D * (x+1)(y+1) - (D*x) * (y+1) - (D*y) * (x+1) + (D*x*y)
                res += bit1[i][j] * (x + 1) * (y + 1) - bit2[i][j] * (y + 1) - bit3[i][j] * (x + 1) + bit4[i][j];
            }
        }
        return res;
    }

public:
    FenwickTree2DRange(int rows, int cols)
    {
        n = rows;
        m = cols;
        // 1-indexed, so allocate size + 1
        bit1.assign(n + 1, vector<long long>(m + 1, 0));
        bit2.assign(n + 1, vector<long long>(m + 1, 0));
        bit3.assign(n + 1, vector<long long>(m + 1, 0));
        bit4.assign(n + 1, vector<long long>(m + 1, 0));
    }

    // Add `val` to the rectangle from (x1, y1) to (x2, y2)
    void update(int x1, int y1, int x2, int y2, long long val)
    {
        // 2D Difference Array Inclusion-Exclusion
        add(x1, y1, val);
        add(x2 + 1, y1, -val);
        add(x1, y2 + 1, -val);
        add(x2 + 1, y2 + 1, val);
    }

    // Get the sum of the rectangle from (x1, y1) to (x2, y2)
    long long query(int x1, int y1, int x2, int y2)
    {
        // 2D Prefix Sum Inclusion-Exclusion
        return queryPrefix(x2, y2) - queryPrefix(x1 - 1, y2) - queryPrefix(x2, y1 - 1) + queryPrefix(x1 - 1, y1 - 1);
    }
};

void solve()
{
    int n, m, q;
    // Example Input Format: N rows, M cols, Q queries
    if (!(cin >> n >> m >> q))
        return;

    FenwickTree2DRange fenwick(n, m);

    while (q--)
    {
        int type;
        cin >> type;

        if (type == 1)
        { // Range Add
            int x1, y1, x2, y2;
            long long val;
            cin >> x1 >> y1 >> x2 >> y2 >> val;
            fenwick.update(x1, y1, x2, y2, val);
        }
        else if (type == 2)
        { // Range Query
            int x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
            cout << fenwick.query(x1, y1, x2, y2) << "\n";
        }
    }
}

int main()
{
    // Crucial fast I/O setup for dense CP test cases
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}