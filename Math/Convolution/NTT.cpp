/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: NUMBER THEORETIC TRANSFORM (NTT)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Polynomial multiplication", "Convolution", "Sum of pairs".
 * - Classic Scenarios: You are given two arrays A and B and need to find C
 *   where C[k] = sum(A[i] * B[k-i]) modulo 998244353.
 * - The Magic: A naive double loop takes O(N^2) and TLEs for N = 10^5.
 *   NTT evaluates the polynomials at primitive roots of unity, multiplies
 *   the point values in O(N), and interpolates them back in O(N log N).
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       vector<long long> C = multiply(A, B);
 *
 * - Complexity:
 *       Time: O(N log N)
 *       Space: O(N)
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Modulo: This template is strictly hardcoded for MOD = 998244353 and
 *   its primitive root 3. If the problem has a different modulo, you MUST
 *   use an Arbitrary Modulo NTT (MTT) or find the primitive root of the new modulo.
 */

using ll = long long;
const ll MOD = 998244353;
const ll ROOT = 3;

ll binpow(ll a, ll b)
{
    ll res = 1;
    a %= MOD;
    while (b > 0)
    {
        if (b & 1)
            res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

ll modInverse(ll n)
{
    return binpow(n, MOD - 2);
}

void ntt(vector<ll> &a, bool invert)
{
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++)
    {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j)
            swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1)
    {
        ll wlen = binpow(ROOT, (MOD - 1) / len);
        if (invert)
            wlen = modInverse(wlen);
        for (int i = 0; i < n; i += len)
        {
            ll w = 1;
            for (int j = 0; j < len / 2; j++)
            {
                ll u = a[i + j];
                ll v = a[i + j + len / 2] * w % MOD;
                a[i + j] = (u + v < MOD ? u + v : u + v - MOD);
                a[i + j + len / 2] = (u - v >= 0 ? u - v : u - v + MOD);
                w = w * wlen % MOD;
            }
        }
    }
    if (invert)
    {
        ll n_inv = modInverse(n);
        for (ll &x : a)
            x = x * n_inv % MOD;
    }
}

vector<ll> multiply(vector<ll> const &a, vector<ll> const &b)
{
    vector<ll> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    int n = 1;
    while (n < a.size() + b.size())
        n <<= 1;
    fa.resize(n);
    fb.resize(n);
    ntt(fa, false);
    ntt(fb, false);
    for (int i = 0; i < n; i++)
        fa[i] = fa[i] * fb[i] % MOD;
    ntt(fa, true);
    while (fa.size() > 1 && fa.back() == 0)
        fa.pop_back();
    return fa;
}