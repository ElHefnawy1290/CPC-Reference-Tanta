/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: POLYNOMIAL EXP, POW, & SQRT
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Exponentiate Generating Function", "e^{A(x)}", "A(x)^K", "sqrt(A(x))".
 * - Classic Scenarios: Solving combinatorial problems involving unlabelled sets
 *   or Euler transforms (Topic 335, 337, 338).
 * - The Magic: Polynomial Exp uses Newton's method on logarithms:
 *   F_{k+1} = F_k * (1 - ln(F_k) + A). Polynomial Pow uses the identity
 *   A^k = exp(k * ln(A)).
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       vector<ll> expA = poly_exp(A, N);
 *       vector<ll> powA = poly_pow(A, K, N);
 *       vector<ll> sqrtA = poly_sqrt(A, N);
 *
 * - Complexity:
 *       Time: O(N log N)
 *       Space: O(N)
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Dependencies: Requires all functions from FPS Toolkit Part 1.
 * - Constant Term Strictness:
 *     - poly_exp: A[0] MUST be 0.
 *     - poly_pow: A[0] MUST be 1 (if not, factor out the lowest degree term first).
 *     - poly_sqrt: A[0] MUST be 1.
 */

vector<ll> poly_exp(vector<ll> a, int deg)
{
    if (deg == 1)
        return {1};
    vector<ll> b = poly_exp(a, (deg + 1) / 2);
    b.resize(deg);
    vector<ll> lnb = poly_log(b, deg);
    vector<ll> diff(deg);
    for (int i = 0; i < deg; i++)
    {
        diff[i] = (a.size() > i ? a[i] : 0) - lnb[i];
        if (diff[i] < 0)
            diff[i] += MOD;
    }
    diff[0] = (diff[0] + 1) % MOD;
    vector<ll> res = multiply(b, diff);
    res.resize(deg);
    return res;
}

vector<ll> poly_pow(vector<ll> a, ll k, int deg)
{
    vector<ll> lna = poly_log(a, deg);
    for (int i = 0; i < lna.size(); i++)
    {
        lna[i] = lna[i] * (k % MOD) % MOD;
    }
    return poly_exp(lna, deg);
}

vector<ll> poly_sqrt(vector<ll> a, int deg)
{
    if (deg == 1)
        return {1};
    vector<ll> b = poly_sqrt(a, (deg + 1) / 2);
    b.resize(deg);
    vector<ll> inv_b = poly_inv(b, deg);
    vector<ll> res = multiply(a, inv_b);
    ll inv2 = modInverse(2);
    res.resize(deg);
    for (int i = 0; i < deg; i++)
    {
        res[i] = (res[i] + b[i]) % MOD * inv2 % MOD;
    }
    return res;
}