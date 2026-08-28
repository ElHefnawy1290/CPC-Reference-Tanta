/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: SUBSET CONVOLUTION
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Disjoint subset sum", "i OR j = k AND i AND j = 0".
 * - Classic Scenarios: Combining two DP states where the masks must perfectly
 *   interlock (share no common set bits) to form a target mask.
 * - The Magic: By adding an extra dimension for the `__builtin_popcount`, we
 *   guarantee that if two masks combine to form a mask of size K, their individual
 *   set bits must sum to exactly K, implicitly enforcing that they are disjoint.
 *   It runs SOS DP on each popcount layer.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       vector<long long> C = subset_convolution(A, B, N, MOD);
 *
 * - Complexity:
 *       Time: O(N^2 * 2^N)
 *       Space: O(N * 2^N)
 */

using ll = long long;

vector<ll> subset_convolution(const vector<ll> &a, const vector<ll> &b, int n, ll mod)
{
    vector<vector<ll>> f_hat(n + 1, vector<ll>(1 << n, 0));
    vector<vector<ll>> g_hat(n + 1, vector<ll>(1 << n, 0));
    vector<vector<ll>> h_hat(n + 1, vector<ll>(1 << n, 0));

    for (int mask = 0; mask < (1 << n); mask++)
    {
        int pc = __builtin_popcount(mask);
        f_hat[pc][mask] = a[mask] % mod;
        g_hat[pc][mask] = b[mask] % mod;
    }

    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int mask = 0; mask < (1 << n); mask++)
            {
                if (mask & (1 << j))
                {
                    f_hat[i][mask] = (f_hat[i][mask] + f_hat[i][mask ^ (1 << j)]) % mod;
                    g_hat[i][mask] = (g_hat[i][mask] + g_hat[i][mask ^ (1 << j)]) % mod;
                }
            }
        }
    }

    for (int mask = 0; mask < (1 << n); mask++)
    {
        for (int i = 0; i <= n; i++)
        {
            ll sum = 0;
            for (int j = 0; j <= i; j++)
            {
                sum = (sum + f_hat[j][mask] * g_hat[i - j][mask]) % mod;
            }
            h_hat[i][mask] = sum;
        }
    }

    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int mask = 0; mask < (1 << n); mask++)
            {
                if (mask & (1 << j))
                {
                    h_hat[i][mask] = (h_hat[i][mask] - h_hat[i][mask ^ (1 << j)] + mod) % mod;
                }
            }
        }
    }

    vector<ll> c(1 << n);
    for (int mask = 0; mask < (1 << n); mask++)
    {
        c[mask] = h_hat[__builtin_popcount(mask)][mask];
    }
    return c;
}