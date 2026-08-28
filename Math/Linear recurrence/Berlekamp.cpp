/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: BERLEKAMP-MASSEY
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Find the recurrence", "Sequence guesser".
 * - Classic Scenarios: You brute-forced the first 50 terms of a DP or counting
 *   problem, but you cannot figure out the actual mathematical recurrence relation.
 * - The Magic: It iteratively builds a polynomial that perfectly models the
 *   input sequence. If the true recurrence has length N, giving BM the first
 *   2N terms guarantees it will perfectly reconstruct the formula.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       vector<ll> C = berlekamp_massey(S, MOD);
 *       // S is the sequence you brute-forced.
 *       // C contains the coefficients: S[i] = C[0]*S[i-1] + C[1]*S[i-2] + ...
 *
 * - Complexity:
 *       Time: O(|S|^2)
 *       Space: O(|S|)
 */

using ll = long long;

ll binpow_bm(ll a, ll b, ll mod)
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

ll modInverse_bm(ll n, ll mod)
{
    return binpow_bm(n, mod - 2, mod);
}

vector<ll> berlekamp_massey(const vector<ll> &s, ll mod)
{
    vector<ll> c = {1}, b = {1};
    ll L = 0, m = 1, b_val = 1;
    for (int i = 0; i < s.size(); i++)
    {
        ll d = 0;
        for (int j = 0; j <= L; j++)
        {
            d = (d + c[j] * s[i - j]) % mod;
        }
        if (d == 0)
        {
            m++;
        }
        else
        {
            vector<ll> temp = c;
            ll c_val = (d * modInverse_bm(b_val, mod)) % mod;
            while (c.size() <= b.size() + m)
                c.push_back(0);
            for (int j = 0; j < b.size(); j++)
            {
                c[j + m] = (c[j + m] - c_val * b[j] % mod + mod) % mod;
            }
            if (2 * L <= i)
            {
                L = i + 1 - L;
                b = temp;
                b_val = d;
                m = 1;
            }
            else
            {
                m++;
            }
        }
    }
    c.erase(c.begin());
    for (ll &x : c)
        x = (mod - x) % mod;
    return c;
}