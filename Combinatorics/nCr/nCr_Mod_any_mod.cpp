/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: nCr MODULO ANY MOD
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "nCr", "Composite Modulo", "No Modular Inverse".
 * - Classic Scenarios: Compute nCr(N, K) modulo M where M is NOT a prime number
 *   (e.g., M = 10^9, or some random composite).
 * - The Magic: Since M is composite, you cannot divide by factorials (no inverse).
 *   This algorithm factors M into prime powers p^k. It then counts the exact
 *   number of times p divides N!, K!, and (N-K)!, isolates the coprime parts,
 *   calculates the binomial coefficient modulo p^k, and finally uses the Chinese
 *   Remainder Theorem (CRT) to stitch them all back together into a single answer.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       long long ans = nCr_arbitrary_mod(N, K, M);
 *
 * - Complexity:
 *       Time: O(sum(p^k) + log N). Extremely fast for M <= 10^6.
 *       Space: O(sum(p^k)) to store factorials modulo p^k.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Dependencies: This heavily relies on your existing `extgcd` and `crt` templates
 *   from your Modular Arithmetic section. Make sure they are included above this.
 */

using ll = long long;

ll custom_pow(ll base, ll exp, ll mod)
{
    ll res = 1;
    base %= mod;
    while (exp > 0)
    {
        if (exp % 2 == 1)
            res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

ll extgcd(ll a, ll b, ll &x, ll &y);
pair<ll, ll> crt(const vector<ll> &a, const vector<ll> &m);

ll modInverse(ll a, ll m)
{
    ll x, y;
    extgcd(a, m, x, y);
    return (x % m + m) % m;
}

ll fact_mod_pk(ll n, ll p, ll pk, int &e)
{
    e = 0;
    if (n == 0)
        return 1;
    ll res = 1;
    for (ll i = 2; i <= pk; i++)
    {
        if (i % p != 0)
            res = (res * i) % pk;
    }
    res = custom_pow(res, n / pk, pk);
    for (ll i = 2; i <= n % pk; i++)
    {
        if (i % p != 0)
            res = (res * i) % pk;
    }
    int next_e;
    res = (res * fact_mod_pk(n / p, p, pk, next_e)) % pk;
    e += n / p + next_e;
    return res;
}

ll nCr_pk(ll n, ll r, ll p, ll pk)
{
    if (n < r || r < 0)
        return 0;
    int e1, e2, e3;
    ll num = fact_mod_pk(n, p, pk, e1);
    ll den1 = fact_mod_pk(r, p, pk, e2);
    ll den2 = fact_mod_pk(n - r, p, pk, e3);
    ll p_pow = custom_pow(p, e1 - e2 - e3, pk);
    if (p_pow == 0)
        return 0;
    ll ans = (num * modInverse((den1 * den2) % pk, pk)) % pk;
    return (ans * p_pow) % pk;
}

ll nCr_arbitrary_mod(ll n, ll r, ll m)
{
    if (n < r || r < 0)
        return 0;
    if (m == 1)
        return 0;
    vector<ll> rem, mod;
    ll temp = m;
    for (ll i = 2; i * i <= temp; i++)
    {
        if (temp % i == 0)
        {
            ll pk = 1;
            while (temp % i == 0)
            {
                pk *= i;
                temp /= i;
            }
            rem.push_back(nCr_pk(n, r, i, pk));
            mod.push_back(pk);
        }
    }
    if (temp > 1)
    {
        rem.push_back(nCr_pk(n, r, temp, temp));
        mod.push_back(temp);
    }
    return crt(rem, mod).first;
}