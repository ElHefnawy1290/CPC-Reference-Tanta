/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: POWER TOWER (GENERALIZED EULER)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Power Tower", "A^B^C^D mod M".
 * - Classic Scenarios: You are given an array of numbers and asked to compute their
 *   power tower (a[0] ^ (a[1] ^ (a[2] ^ ... ))) modulo M.
 * - The Magic: It applies the Generalized Euler Theorem to safely reduce exponents
 *   using the Phi function, effectively capping the recursion depth at O(log M).
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       long long ans = power_tower(arr, 0, M);
 *
 * - Complexity:
 *       Time: O(log M * sqrt(M))
 *       Space: O(log M) for the recursion stack.
 */

using ll = long long;

ll phi(ll n)
{
    ll res = n;
    for (ll i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
        {
            while (n % i == 0)
                n /= i;
            res -= res / i;
        }
    }
    if (n > 1)
        res -= res / n;
    return res;
}

ll custom_pow(ll base, ll exp, ll mod)
{
    ll res = 1;
    base = (base >= mod) ? (base % mod + mod) : base;
    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            res *= base;
            res = (res >= mod) ? (res % mod + mod) : res;
        }
        base *= base;
        base = (base >= mod) ? (base % mod + mod) : base;
        exp /= 2;
    }
    return res;
}

ll power_tower(const vector<ll> &arr, int idx, ll mod)
{
    if (idx == arr.size() || mod == 1)
        return 1;
    if (arr[idx] == 1)
        return 1;
    ll next_mod = phi(mod);
    ll exponent = power_tower(arr, idx + 1, next_mod);
    return custom_pow(arr[idx], exponent, mod);
}