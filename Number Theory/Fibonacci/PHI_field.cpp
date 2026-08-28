/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: PHI FIELD (Z[sqrt(5)])
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Fibonacci sum of powers", "Binet's formula modulo M".
 * - Classic Scenarios: You need to evaluate algebraic expressions involving
 *   Fibonacci numbers, such as sum(F_i^K) mod M.
 * - The Magic: It operates exactly like complex numbers, but instead of i^2 = -1,
 *   it maps a + b*sqrt(5) under modulo M. This allows you to directly use Binet's
 *   formula natively even if 5 is a quadratic non-residue modulo M!
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       Phi res = power_phi({A, B}, exp, MOD);
 *
 * - Complexity:
 *       Time: O(log(exp)) per multiplication.
 *       Space: O(1)
 */

using ll = long long;

struct Phi
{
    ll a, b;
};

Phi mult_phi(Phi x, Phi y, ll mod)
{
    return {
        (x.a * y.a + 5 * x.b % mod * y.b) % mod,
        (x.a * y.b + x.b * y.a) % mod};
}

Phi power_phi(Phi base, ll exp, ll mod)
{
    Phi res = {1, 0};
    base.a %= mod;
    base.b %= mod;
    while (exp > 0)
    {
        if (exp % 2 == 1)
            res = mult_phi(res, base, mod);
        base = mult_phi(base, base, mod);
        exp /= 2;
    }
    return res;
}