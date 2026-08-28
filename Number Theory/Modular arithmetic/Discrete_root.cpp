/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: DISCRETE ROOT (K-TH ROOT)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "K-th Root Modulo P", "x^K = A mod P".
 * - Classic Scenarios: You need to reverse an exponentiation where the modulo is prime.
 * - The Magic: It finds a primitive root g of P. Since x can be represented as g^y,
 *   the equation becomes (g^y)^K ≡ A (mod P) => (g^K)^y ≡ A (mod P). This perfectly
 *   reduces the problem into a Discrete Logarithm (BSGS) problem!
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       long long root = discrete_root(K, A, P);
 *       // Returns -1 if no root exists.
 *
 * - Complexity:
 *       Time: O(sqrt(P) * log(sqrt(P))).
 *       Space: O(sqrt(P)).
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Dependencies: This snippet requires both `custom_pow` and `solve_bsgs` to be
 *   available in your code (from the snippets above).
 */

using ll = long long;

ll primitive_root(ll p)
{
    vector<ll> fact;
    ll phi = p - 1, n = phi;
    for (ll i = 2; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            fact.push_back(i);
            while (n % i == 0)
                n /= i;
        }
    }
    if (n > 1)
        fact.push_back(n);

    for (ll res = 2; res <= p; ++res)
    {
        bool ok = true;
        for (size_t i = 0; i < fact.size() && ok; ++i)
        {
            ok &= custom_pow(res, phi / fact[i], p) != 1;
        }
        if (ok)
            return res;
    }
    return -1;
}

ll discrete_root(ll k, ll a, ll p)
{
    if (a == 0)
        return 0;
    if (k == 0)
        return (a == 1) ? 1 : -1;

    ll g = primitive_root(p);
    if (g == -1)
        return -1;

    ll base = custom_pow(g, k, p);
    ll y = solve_bsgs(base, a, p);
    if (y == -1)
        return -1;

    return custom_pow(g, y, p);
}