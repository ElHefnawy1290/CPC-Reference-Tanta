/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: 2D SPARSE TABLE
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Static 2D Grid", "Maximum in subgrid", "Minimum in subgrid", "Huge number of queries".
 * - Classic Scenarios: You are given an N x M grid of numbers. The grid NEVER changes.
 *   You need to answer Q queries (up to 10^6) asking for the max/min in a rectangle bounded
 *   by top-left (r1, c1) and bottom-right (r2, c2).
 * - The Magic: A 2D Segment tree answers queries in O(log N * log M), which can Time Limit Exceed
 *   (TLE) if Q is massive. A 2D Sparse Table builds the answers for all powers of 2 in advance,
 *   allowing it to answer ANY rectangle query in absolute O(1) time by overlapping 4 smaller rectangles!
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Pass a 0-indexed 2D vector to the constructor.
 *       vector<vector<int>> grid = {{1, 5}, {2, 8}};
 *       SparseTable2D st(grid);
 *
 * - Queries (All 0-indexed, Inclusive [r1, r2] and [c1, c2]):
 *       // Query the max in the subgrid from top-left (0, 0) to bottom-right (1, 1)
 *       int max_val = st.query(0, 0, 1, 1);
 *
 * - Complexity:
 *       Time: O(N * M * log N * log M) to build. Pure O(1) per query.
 *       Space: O(N * M * log N * log M) memory.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Changing from Max to Min or GCD?
 *   1. Change the `combine()` function to return `min(a, b)` or `std::gcd(a, b)`.
 * - Memory Limit Exceeded (MLE) Warning:
 *   If N = 1000 and M = 1000, log(N) = 10. The table requires 1000 * 1000 * 10 * 10 = 10^8 ints.
 *   This is ~400 MB of memory. This passes on most platforms (512MB limits), but if the limit
 *   is strictly 256MB, you will MLE. Always check the memory limit before choosing this!
 * - Idempotent Operations ONLY:
 *   Sparse tables ONLY work for operations where X + X = X (like Min, Max, GCD, Bitwise OR/AND).
 *   Do NOT use this for Sum queries! (Use a 2D Prefix Sum array for O(1) sums instead).
 */

#include <bits/stdc++.h>
using namespace std;

// struct SparseTable2D {
private:
int n, m;
int log_n, log_m;
// table[ir][ic][r][c]
// Represents the rectangle of size 2^ir x 2^ic starting at top-left (r, c)
vector<vector<vector<vector<int>>>> table;

// Precomputed logs for pure O(1) queries
vector<int> log_table;

// ? Dial 1: The Combination Logic (Change to min, gcd, |, &, etc.)
inline int combine(int a, int b)
{
    return max(a, b);
}

// public:
SparseTable2D(const vector<vector<int>> &grid)
{
    if (grid.empty() || grid[0].empty())
        return;
    n = grid.size();
    m = grid[0].size();

    // Precompute logarithms
    int max_dim = max(n, m);
    log_table.assign(max_dim + 1, 0);
    for (int i = 2; i <= max_dim; i++)
    {
        log_table[i] = log_table[i / 2] + 1;
    }

    log_n = log_table[n] + 1;
    log_m = log_table[m] + 1;

    // Allocate memory carefully: ir -> ic -> row -> col
    // This specific ordering prevents massive cache-miss penalties during the build phase
    table.assign(log_n, vector<vector<vector<int>>>(log_m,
                                                    vector<vector<int>>(n, vector<int>(m))));

    // 1. Base Case: 1x1 rectangles (2^0 x 2^0)
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            table[0][0][i][j] = grid[i][j];
        }
    }

    // 2. Build purely horizontally (1D Sparse Table for each row)
    for (int ir = 0; ir < log_n; ir++)
    {
        for (int ic = 0; ic < log_m; ic++)
        {
            // Skip the base case we already filled
            if (ir == 0 && ic == 0)
                continue;

            for (int i = 0; i + (1 << ir) <= n; i++)
            {
                for (int j = 0; j + (1 << ic) <= m; j++)
                {

                    if (ir == 0)
                    {
                        // Merge two adjacent horizontal blocks
                        table[ir][ic][i][j] = combine(
                            table[ir][ic - 1][i][j],
                            table[ir][ic - 1][i][j + (1 << (ic - 1))]);
                    }
                    else
                    {
                        // Merge two adjacent vertical blocks
                        table[ir][ic][i][j] = combine(
                            table[ir - 1][ic][i][j],
                            table[ir - 1][ic][i + (1 << (ir - 1))][j]);
                    }
                }
            }
        }
    }
}

//    // Query inclusive rectangle from top-left (r1, c1) to bottom-right (r2, c2)
int query(int r1, int c1, int r2, int c2)
{
    // Find the largest power of 2 that fits inside the row and column lengths
    int k_r = log_table[r2 - r1 + 1];
    int k_c = log_table[c2 - c1 + 1];

    // ? THE MAGIC: We perfectly cover the requested rectangle using 4 smaller overlapping
    // rectangles of size (2^k_r) x (2^k_c).
    // Because the operation is Idempotent (max/min), overlapping data doesn't change the answer!

    int top_left = table[k_r][k_c][r1][c1];
    int top_right = table[k_r][k_c][r1][c2 - (1 << k_c) + 1];
    int bottom_left = table[k_r][k_c][r2 - (1 << k_r) + 1][c1];
    int bottom_right = table[k_r][k_c][r2 - (1 << k_r) + 1][c2 - (1 << k_c) + 1];

    return combine(combine(top_left, top_right), combine(bottom_left, bottom_right));
}
}
;

// void solve() {
int n, m, q;
// Example Input: N rows, M cols, Q queries
if (!(cin >> n >> m >> q))
    return;

vector<vector<int>> grid(n, vector<int>(m));
for (int i = 0; i < n; i++)
{
    for (int j = 0; j < m; j++)
    {
        cin >> grid[i][j];
    }
}

SparseTable2D st(grid);

while (q--)
{
    int r1, c1, r2, c2;
    // Assume 0-indexed coordinates are provided
    cin >> r1 >> c1 >> r2 >> c2;
    cout << st.query(r1, c1, r2, c2) << "\n";
}
}

int main()
{
    // Fast I/O is mandatory for heavy 2D queries
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}