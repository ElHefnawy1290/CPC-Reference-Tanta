/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: DETERMINANT (PRIME MODULO)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Matrix Tree Theorem", "Number of Spanning Trees".
 * - Classic Scenarios: You have an N x N matrix and need its determinant modulo
 *   a prime number like 1e9+7 or 998244353.
 * - The Magic: Standard Gaussian elimination. It tracks row swaps because every
 *   swap multiplies the final determinant by -1.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       long long ans = det_prime(matrix, MOD);
 *
 * - Complexity:
 *       Time: O(N^3)
 *       Space: O(N^2)
 */

using ll = long long;

ll binpow_det(ll a, ll b, ll mod)
{
    ll res = 1;
    a %= mod;
    while (b > 0)
    {
        if (b & 1)
            res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}

ll modInverse_det(ll n, ll mod)
{
    return binpow_det(n, mod - 2, mod);
}

ll det_prime(vector<vector<ll>> a, ll mod)
{
    int n = a.size();
    ll ans = 1;
    for (int i = 0; i < n; ++i)
    {
        int pivot = i;
        for (int j = i + 1; j < n; ++j)
        {
            if (a[j][i] > a[pivot][i])
                pivot = j;
        }
        if (a[pivot][i] == 0)
            return 0;
        if (i != pivot)
        {
            swap(a[i], a[pivot]);
            ans = (mod - ans) % mod;
        }
        ans = ans * a[i][i] % mod;
        ll inv = modInverse_det(a[i][i], mod);
        for (int j = i + 1; j < n; ++j)
        {
            ll factor = a[j][i] * inv % mod;
            for (int k = i; k < n; ++k)
            {
                a[j][k] = (a[j][k] - factor * a[i][k]) % mod;
                if (a[j][k] < 0)
                    a[j][k] += mod;
            }
        }
    }
    return ans;
}