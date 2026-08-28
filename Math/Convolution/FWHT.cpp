/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: FAST WALSH-HADAMARD TRANSFORM
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Bitwise convolution", "XOR sum", "AND sum", "OR sum".
 * - Classic Scenarios: Find array C where C[k] = sum(A[i] * B[j]) for all
 *   pairs (i, j) such that (i XOR j) == k.
 * - The Magic: It alters the NTT butterfly structure to map binary bitwise
 *   operations into addition/subtraction, resolving the convolution in O(N log N).
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       vector<long long> C = fwht_xor(A, B);
 *       // Swap 'fwht_xor' for 'fwht_and' or 'fwht_or' as needed.
 *
 * - Complexity:
 *       Time: O(N log N). Note that N MUST be a power of 2.
 *       Space: O(N).
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Padding: The input arrays MUST be padded with zeros until their size
 *   is exactly a power of 2 before passing them into these functions.
 */

using ll = long long;
const ll MOD = 998244353;

ll binpow_fwht(ll a, ll b)
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

ll modInv(ll n) { return binpow_fwht(n, MOD - 2); }

void fwht_xor_base(vector<ll> &a, bool inv)
{
    int n = a.size();
    for (int len = 1; 2 * len <= n; len <<= 1)
    {
        for (int i = 0; i < n; i += 2 * len)
        {
            for (int j = 0; j < len; j++)
            {
                ll u = a[i + j];
                ll v = a[i + len + j];
                a[i + j] = (u + v) % MOD;
                a[i + len + j] = (u - v + MOD) % MOD;
            }
        }
    }
    if (inv)
    {
        ll invN = modInv(n);
        for (ll &x : a)
            x = x * invN % MOD;
    }
}

void fwht_and_base(vector<ll> &a, bool inv)
{
    int n = a.size();
    for (int len = 1; 2 * len <= n; len <<= 1)
    {
        for (int i = 0; i < n; i += 2 * len)
        {
            for (int j = 0; j < len; j++)
            {
                ll u = a[i + j];
                ll v = a[i + len + j];
                if (!inv)
                {
                    a[i + j] = (u + v) % MOD;
                }
                else
                {
                    a[i + j] = (u - v + MOD) % MOD;
                }
            }
        }
    }
}

void fwht_or_base(vector<ll> &a, bool inv)
{
    int n = a.size();
    for (int len = 1; 2 * len <= n; len <<= 1)
    {
        for (int i = 0; i < n; i += 2 * len)
        {
            for (int j = 0; j < len; j++)
            {
                ll u = a[i + j];
                ll v = a[i + len + j];
                if (!inv)
                {
                    a[i + len + j] = (u + v) % MOD;
                }
                else
                {
                    a[i + len + j] = (v - u + MOD) % MOD;
                }
            }
        }
    }
}

vector<ll> fwht_xor(vector<ll> a, vector<ll> b)
{
    fwht_xor_base(a, false);
    fwht_xor_base(b, false);
    for (size_t i = 0; i < a.size(); i++)
        a[i] = a[i] * b[i] % MOD;
    fwht_xor_base(a, true);
    return a;
}

vector<ll> fwht_and(vector<ll> a, vector<ll> b)
{
    fwht_and_base(a, false);
    fwht_and_base(b, false);
    for (size_t i = 0; i < a.size(); i++)
        a[i] = a[i] * b[i] % MOD;
    fwht_and_base(a, true);
    return a;
}

vector<ll> fwht_or(vector<ll> a, vector<ll> b)
{
    fwht_or_base(a, false);
    fwht_or_base(b, false);
    for (size_t i = 0; i < a.size(); i++)
        a[i] = a[i] * b[i] % MOD;
    fwht_or_base(a, true);
    return a;
}