/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: PERMANENT OF A MATRIX (RYSER'S FORMULA)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Number of perfect matchings", "Bipartite graph matchings".
 * - Classic Scenarios: Given an N x N adjacency matrix of a bipartite graph,
 *   find exactly how many distinct perfect matchings exist.
 * - The Magic: Uses Inclusion-Exclusion (Ryser's Formula) to bypass the
 *   factorial complexity of permutations, solving it in exactly O(N * 2^N).
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       long long ways = permanent(adj_matrix);
 *
 * - Complexity:
 *       Time: O(N * 2^N) — Generally safe for N <= 20.
 *       Space: O(N)
 */

using ll = long long;
const ll MOD = 1e9 + 7;

ll permanent(const vector<vector<ll>> &a)
{
    int n = a.size();
    if (n == 0)
        return 0;

    ll total_perm = 0;
    int max_mask = 1 << n;

    for (int mask = 1; mask < max_mask; mask++)
    {
        ll current_prod = 1;
        int set_bits = 0;

        for (int i = 0; i < n; i++)
        {
            ll row_sum = 0;
            int bit_count = 0;
            for (int j = 0; j < n; j++)
            {
                if ((mask >> j) & 1)
                {
                    row_sum = (row_sum + a[i][j]) % MOD;
                    if (i == 0)
                        bit_count++;
                }
            }
            if (i == 0)
                set_bits = bit_count;
            current_prod = (current_prod * row_sum) % MOD;
        }

        if ((n - set_bits) % 2 == 1)
        {
            total_perm = (total_perm - current_prod + MOD) % MOD;
        }
        else
        {
            total_perm = (total_perm + current_prod) % MOD;
        }
    }
    return total_perm;
}