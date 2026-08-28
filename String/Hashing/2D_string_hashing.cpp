/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: 2D STRING HASHING
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Subgrid matching", "2D Pattern matching".
 * - Classic Scenarios: Find how many times an R x C pattern appears inside
 *   an N x M grid.
 * - The Magic: Extends the rolling hash to two dimensions using two different
 *   bases (one for rows, one for columns). Calculates 2D prefix sums of the hash.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization:
 *       Hash2D h(grid);
 * - Query:
 *       ll val = h.get(r1, c1, r2, c2); // 0-indexed, inclusive boundaries
 */

using ll = long long;

struct Hash2D
{
    const ll MOD = 1e9 + 7;
    const ll BASE_R = 313, BASE_C = 317;
    vector<vector<ll>> hash;
    vector<ll> pR, pC;

    Hash2D(const vector<string> &grid)
    {
        if (grid.empty())
            return;
        int n = grid.size();
        int m = grid[0].size();

        hash.assign(n + 1, vector<ll>(m + 1, 0));
        pR.assign(n + 1, 1);
        pC.assign(m + 1, 1);

        for (int i = 1; i <= n; i++)
            pR[i] = (pR[i - 1] * BASE_R) % MOD;
        for (int i = 1; i <= m; i++)
            pC[i] = (pC[i - 1] * BASE_C) % MOD;

        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= m; j++)
            {
                hash[i][j] = (hash[i - 1][j] * BASE_R + hash[i][j - 1] * BASE_C - hash[i - 1][j - 1] * BASE_R % MOD * BASE_C % MOD + grid[i - 1][j - 1]) % MOD;
                if (hash[i][j] < 0)
                    hash[i][j] += MOD;
            }
        }
    }

    ll get(int r1, int c1, int r2, int c2)
    {
        r1++;
        c1++;
        r2++;
        c2++;
        ll h = hash[r2][c2];
        h = (h - hash[r1 - 1][c2] * pR[r2 - r1 + 1]) % MOD;
        h = (h - hash[r2][c1 - 1] * pC[c2 - c1 + 1]) % MOD;
        h = (h + hash[r1 - 1][c1 - 1] * pR[r2 - r1 + 1] % MOD * pC[c2 - c1 + 1]) % MOD;
        if (h < 0)
            h += MOD;
        return h;
    }
};