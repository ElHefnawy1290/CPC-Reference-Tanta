/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: BROKEN PROFILE DP (PLUG DP)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Tile the grid", "Dominoes", "Hamiltonian Path on Grid".
 * - Classic Scenarios: Find the number of ways to completely cover an N x M grid
 *   using 1x2 dominoes where N is huge (10^5) and M is very small (<= 15).
 * - The Magic: It sweeps through the grid cell by cell (not row by row).
 *   It maintains a bitmask of length M representing the "profile" (the boundary
 *   of cells protruding into the current row). As we move to the next cell, we
 *   shift the mask and try placing dominoes horizontally or vertically.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       long long ways = plug_dp_dominoes(N, M, MOD);
 *
 * - Complexity:
 *       Time: O(N * M * 2^M)
 *       Space: O(2^M) using rolling arrays.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Obstacles: If the grid has blocked cells, simply check if the current cell
 *   (i, j) is blocked. If it is, ensure the current bit in the mask is 0, shift it
 *   to 1 (meaning it's "filled"), and move to the next cell without placing dominoes.
 */

using ll = long long;

ll plug_dp_dominoes(int n, int m, ll mod)
{
    if ((n * m) % 2 != 0)
        return 0; // Odd number of cells cannot be tiled by 1x2

    // We only need the current and next state, so we use a rolling array of size 2
    vector<vector<ll>> dp(2, vector<ll>(1 << m, 0));

    int cur = 0, next = 1;
    dp[cur][0] = 1; // Base case: completely empty profile

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            fill(dp[next].begin(), dp[next].end(), 0);

            for (int mask = 0; mask < (1 << m); mask++)
            {
                if (dp[cur][mask] == 0)
                    continue;

                // Case 1: The current cell is already filled by a vertical domino
                // from the previous row. We must leave it alone. The bit shifts out.
                if (mask & (1 << (m - 1)))
                {
                    int next_mask = (mask ^ (1 << (m - 1))) << 1;
                    dp[next][next_mask] = (dp[next][next_mask] + dp[cur][mask]) % mod;
                }
                // Case 2: The cell is empty. We must place a domino.
                else
                {
                    // Option A: Place a vertical domino (protrudes into the next row).
                    // We set the 0-th bit of the shifted mask to 1.
                    int next_mask_vert = (mask << 1) | 1;
                    dp[next][next_mask_vert] = (dp[next][next_mask_vert] + dp[cur][mask]) % mod;

                    // Option B: Place a horizontal domino.
                    // Only valid if we aren't at the right edge AND the next cell is empty.
                    if (j + 1 < m && !(mask & 1))
                    {
                        // The horizontal domino fills the current and next cell in THIS row.
                        // We shift the mask by 1, but we don't protrude into the next row,
                        // so the 0-th bit remains 0. However, the cell to our right is now filled,
                        // which we must account for in our transition logic by skipping it or
                        // setting its bit. In this cell-by-cell state machine, placing a horizontal
                        // domino effectively sets the bit for the *next* cell in the current row.
                        int next_mask_horiz = (mask << 1) | 2;
                        dp[next][next_mask_horiz] = (dp[next][next_mask_horiz] + dp[cur][mask]) % mod;
                    }
                }
            }
            swap(cur, next);
        }
    }

    // The answer is the number of ways to have a completely flat profile (mask == 0) at the end.
    return dp[cur][0];
}