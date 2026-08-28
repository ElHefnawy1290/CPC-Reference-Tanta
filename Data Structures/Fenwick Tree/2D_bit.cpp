/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: 2D FENWICK TREE (POINT UPDATE & RANGE QUERY)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "2D Grid", "Update a single cell", "Sum of subgrid", "Rectangle Queries".
 * - Classic Scenarios: You have an N x M grid. You need to repeatedly change the value of
 *   a specific cell (X, Y). Then, you need to query the sum of all elements inside a
 *   subgrid bounded by top-left (X1, Y1) and bottom-right (X2, Y2).
 * - The Magic: Replaces a bulky, complex 2D Segment Tree. It computes 2D prefix sums dynamically
 *   in O(log N * log M) time using ridiculously short and fast bitwise loops.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Pass the dimensions (rows, cols) to the constructor.
 *   (Note: Fenwick Trees MUST be 1-indexed. The bounds are [1, N] and [1, M]).
 *       FenwickTree2D fenwick(N, M);
 *
 * - Point Updates (1-indexed (x, y)):
 *       // Add 'val' to the cell at row X, col Y
 *       fenwick.update(X, Y, val);
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
 * - "Point Set" instead of "Point Add"?
 *   Fenwick Trees can ONLY add values. If the problem asks you to REPLACE the value at (X, Y)
 *   with `V`, you must maintain a 2D array `grid` of the current values.
 *   Then, do:
 *       long long diff = V - grid[X][Y];
 *       fenwick.update(X, Y, diff);
 *       grid[X][Y] = V;
 *
 * - 0-Indexed Input?
 *   Simply add +1 to all coordinates before passing them into `update` and `query`.
 *   NEVER change the internal 1-based indexing of the BIT.
 */

#include <bits/stdc++.h>
using namespace std;

struct FenwickTree2D
{
private:
    int n, m;
    vector<vector<long long>> bit;

    // Get the sum of the rectangle from (1, 1) to (x, y)
    long long queryPrefix(int x, int y)
    {
        long long sum = 0;
        for (int i = x; i > 0; i -= i & -i)
        {
            for (int j = y; j > 0; j -= j & -j)
            {
                sum += bit[i][j];
            }
        }
        return sum;
    }

public:
    FenwickTree2D(int rows, int cols)
    {
        n = rows;
        m = cols;
        // 1-indexed, so allocate size + 1
        bit.assign(n + 1, vector<long long>(m + 1, 0));
    }

    // Add `val` to the cell at (x, y)
    void update(int x, int y, long long val)
    {
        for (int i = x; i <= n; i += i & -i)
        {
            for (int j = y; j <= m; j += j & -j)
            {
                bit[i][j] += val;
            }
        }
    }

    // Get the sum of the rectangle from (x1, y1) to (x2, y2)
    long long query(int x1, int y1, int x2, int y2)
    {
        // Standard 2D Inclusion-Exclusion Principle
        return queryPrefix(x2, y2) - queryPrefix(x1 - 1, y2) - queryPrefix(x2, y1 - 1) + queryPrefix(x1 - 1, y1 - 1);
    }
};

void solve()
{
    int n, m, q;
    // Example Input Format: N rows, M cols, Q queries
    if (!(cin >> n >> m >> q))
        return;

    FenwickTree2D fenwick(n, m);

    // Optional: Maintain original grid if "Point Set" operations are required
    // vector<vector<long long>> grid(n + 1, vector<long long>(m + 1, 0));

    while (q--)
    {
        int type;
        cin >> type;

        if (type == 1)
        { // Point Add
            int x, y;
            long long val;
            cin >> x >> y >> val;
            fenwick.update(x, y, val);
        }
        else if (type == 2)
        { // Rectangle Query
            int x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
            cout << fenwick.query(x1, y1, x2, y2) << "\n";
        }
    }
}

int main()
{
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}