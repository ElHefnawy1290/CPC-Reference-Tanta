/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: 2D SEGMENT TREE
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Subgrid queries", "Rectangle Sum/Max/Min", "Update a single cell in a grid".
 * - Classic Scenarios: You are given an N x M matrix. You need to repeatedly answer
 *   queries about a specific sub-rectangle (from top-left [R1, C1] to bottom-right [R2, C2]).
 *   Between queries, you can update the value of any specific cell [R, C].
 * - The Magic: A standard Segment Tree handles a 1D array. A 2D Segment Tree is literally
 *   a "Segment Tree of Segment Trees". The outer tree splits the rows, and every single
 *   node in the outer tree contains an inner tree that splits the columns for that row-range.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Pass the row and column sizes to the constructor.
 *   Then, (optionally) build it using an existing 0-indexed 2D vector.
 *       vector<vector<long long>> grid = {{1, 2}, {3, 4}};
 *       SegTree2D st(2, 2);
 *       st.build(grid);
 *
 * - Queries (All 0-indexed, Inclusive [r1, r2] and [c1, c2]):
 *       // Query the sum of the subgrid from top-left (0, 0) to bottom-right (1, 1)
 *       long long total = st.query(0, 0, 1, 1);
 *
 * - Updates (All 0-indexed):
 *       // Change the value at row R, column C to X
 *       st.update(R, C, X);
 *
 * - Complexity:
 *       Time: O(N * M) to build. O(log N * log M) per update and query.
 *       Space: O(N * M) memory. (Uses exactly 16 * N * M elements).
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Changing from Range Sum to Range Min/Max?
 *   1. Change the `NEUTRAL` variable to `1e18` (for Min) or `-1e18` (for Max).
 *   2. Change the `combine()` function to return `min(a, b)` or `max(a, b)`.
 *
 * - Memory Limit Exceeded (MLE) Warning:
 *   If N = 1000 and M = 1000, this tree uses 16,000,000 long longs (~128 MB).
 *   This easily passes standard 256MB limits.
 *   BUT, if N, M > 2000, you will MLE. If that happens and the problem only asks for SUM,
 *   you must use a 2D Fenwick Tree (BIT) instead, which uses 1/16th of the memory!
 */

#include <bits/stdc++.h>
using namespace std;

struct SegTree2D
{
private:
    int n, m;
    vector<vector<long long>> seg;

    // ? Change these two dials to switch between Sum, Min, Max, GCD, etc.
    const long long NEUTRAL = 0;

    long long combine(long long a, long long b)
    {
        return a + b;
    }

    void build_y(int vx, int lx, int rx, int vy, int ly, int ry, const vector<vector<long long>> &a)
    {
        if (ly == ry)
        {
            // We are at a leaf in the column tree.
            if (lx == rx)
            {
                // We are ALSO at a leaf in the row tree. Base case!
                seg[vx][vy] = a[lx][ly];
            }
            else
            {
                // We are a combination of two row-segments at the SAME exact column.
                seg[vx][vy] = combine(seg[vx * 2][vy], seg[vx * 2 + 1][vy]);
            }
        }
        else
        {
            int my = ly + (ry - ly) / 2;
            build_y(vx, lx, rx, vy * 2, ly, my, a);
            build_y(vx, lx, rx, vy * 2 + 1, my + 1, ry, a);
            // Combine the left and right column-halves for the current row-segment
            seg[vx][vy] = combine(seg[vx][vy * 2], seg[vx][vy * 2 + 1]);
        }
    }

    void build_x(int vx, int lx, int rx, const vector<vector<long long>> &a)
    {
        if (lx != rx)
        {
            int mx = lx + (rx - lx) / 2;
            build_x(vx * 2, lx, mx, a);
            build_x(vx * 2 + 1, mx + 1, rx, a);
        }
        // Build the entire inner column-tree for the current row-segment
        build_y(vx, lx, rx, 1, 0, m - 1, a);
    }

    void update_y(int vx, int lx, int rx, int vy, int ly, int ry, int x, int y, long long val)
    {
        if (ly == ry)
        {
            if (lx == rx)
            {
                // Exact cell located!
                seg[vx][vy] = val;
            }
            else
            {
                // Update the combined row-segment value for this specific column
                seg[vx][vy] = combine(seg[vx * 2][vy], seg[vx * 2 + 1][vy]);
            }
        }
        else
        {
            int my = ly + (ry - ly) / 2;
            if (y <= my)
            {
                update_y(vx, lx, rx, vy * 2, ly, my, x, y, val);
            }
            else
            {
                update_y(vx, lx, rx, vy * 2 + 1, my + 1, ry, x, y, val);
            }
            seg[vx][vy] = combine(seg[vx][vy * 2], seg[vx][vy * 2 + 1]);
        }
    }

    void update_x(int vx, int lx, int rx, int x, int y, long long val)
    {
        if (lx != rx)
        {
            int mx = lx + (rx - lx) / 2;
            if (x <= mx)
            {
                update_x(vx * 2, lx, mx, x, y, val);
            }
            else
            {
                update_x(vx * 2 + 1, mx + 1, rx, x, y, val);
            }
        }
        // Force the inner Y-tree to update itself using the new X data
        update_y(vx, lx, rx, 1, 0, m - 1, x, y, val);
    }

    long long query_y(int vx, int vy, int ly, int ry, int qly, int qry)
    {
        if (qly > ry || qry < ly)
            return NEUTRAL;
        if (qly <= ly && ry <= qry)
            return seg[vx][vy];

        int my = ly + (ry - ly) / 2;
        return combine(
            query_y(vx, vy * 2, ly, my, qly, qry),
            query_y(vx, vy * 2 + 1, my + 1, ry, qly, qry));
    }

    long long query_x(int vx, int lx, int rx, int qlx, int qrx, int qly, int qry)
    {
        if (qlx > rx || qrx < lx)
            return NEUTRAL;

        // If the row segment is fully inside our query, hand it off to the column query!
        if (qlx <= lx && rx <= qrx)
        {
            return query_y(vx, 1, 0, m - 1, qly, qry);
        }

        int mx = lx + (rx - lx) / 2;
        return combine(
            query_x(vx * 2, lx, mx, qlx, qrx, qly, qry),
            query_x(vx * 2 + 1, mx + 1, rx, qlx, qrx, qly, qry));
    }

public:
    SegTree2D(int rows, int cols)
    {
        n = rows;
        m = cols;
        // Allocate 4N rows, each containing a 4M column segment tree
        seg.assign(4 * n, vector<long long>(4 * m, NEUTRAL));
    }

    void build(const vector<vector<long long>> &grid)
    {
        build_x(1, 0, n - 1, grid);
    }

    // Update cell at (row, col) to a new exact value
    void update(int row, int col, long long val)
    {
        update_x(1, 0, n - 1, row, col, val);
    }

    // Query inclusive rectangle from top-left (r1, c1) to bottom-right (r2, c2)
    long long query(int r1, int c1, int r2, int c2)
    {
        return query_x(1, 0, n - 1, r1, r2, c1, c2);
    }
};