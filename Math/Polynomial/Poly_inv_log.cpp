/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: POLYNOMIAL INV & LOG
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Generating Functions", "Formal Power Series (FPS)", "Divide Polynomials".
 * - Classic Scenarios: You have a generating function expressed as a fraction A(x)/B(x)
 *   and need the first N coefficients.
 * - The Magic: Polynomial Inverse (Topic 334) uses Newton's Method to find B^{-1}(x)
 *   such that B(x) * B^{-1}(x) = 1 mod x^n in O(N log N). Polynomial Log (Topic 336)
 *   uses calculus: ln(A(x)) = integral( A'(x) * A^{-1}(x) ).
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       vector<ll> invA = poly_inv(A, N);
 *       vector<ll> logA = poly_log(A, N);
 *
 * - Complexity:
 *       Time: O(N log N)
 *       Space: O(N)
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Dependencies: Requires `ntt`, `multiply`, and `modInverse` from your FFT/NTT snippet.
 * - Constant Term: For poly_inv, A[0] MUST NOT be 0. For poly_log, A[0] MUST be 1.
 */

using ll = long long;
const ll MOD = 998244353;

vector<ll> poly_inv(vector<ll> a, int deg)
{
    if (deg == 1)
        return {modInverse(a[0])};
    vector<ll> b = poly_inv(a, (deg + 1) / 2);
    int n = 1;
    while (n < deg * 2)
        n <<= 1;
    vector<ll> fa(a.begin(), a.begin() + min((int)a.size(), deg));
    fa.resize(n);
    vector<ll> fb = b;
    fb.resize(n);
    ntt(fa, false);
    ntt(fb, false);
    for (int i = 0; i < n; i++)
    {
        fb[i] = fb[i] * (2 - fa[i] * fb[i] % MOD + MOD) % MOD;
    }
    ntt(fb, true);
    fb.resize(deg);
    return fb;
}

vector<ll> poly_deriv(const vector<ll> &a)
{
    if (a.empty())
        return {};
    vector<ll> res(max(1, (int)a.size() - 1));
    for (int i = 1; i < a.size(); i++)
    {
        res[i - 1] = a[i] * i % MOD;
    }
    return res;
}

vector<ll> poly_integr(const vector<ll> &a)
{
    if (a.empty())
        return {0};
    vector<ll> res(a.size() + 1, 0);
    for (int i = 0; i < a.size(); i++)
    {
        res[i + 1] = a[i] * modInverse(i + 1) % MOD;
    }
    return res;
}

vector<ll> poly_log(vector<ll> a, int deg)
{
    vector<ll> deriv = poly_deriv(a);
    vector<ll> inv = poly_inv(a, deg);
    vector<ll> res = poly_integr(multiply(deriv, inv));
    res.resize(deg);
    return res;
}