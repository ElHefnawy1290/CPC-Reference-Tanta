/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: CHINESE REMAINDER THEOREM (GENERAL)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "System of congruences", "x = a1 mod m1, x = a2 mod m2".
 * - The Magic: Unlike basic CRT which fails if the moduli are not coprime,
 *   this robust version iteratively combines the equations using the Extended
 *   Euclidean algorithm. It safely catches impossible systems.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       pair<ll, ll> ans = crt(A, M);
 * - Result: Returns {remainder, modulo}. If no solution exists, returns {-1, -1}.
 */

using ll = long long;

ll extgcd_crt(ll a, ll b, ll &x, ll &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }
    ll x1, y1;
    ll d = extgcd_crt(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

pair<ll, ll> crt(const vector<ll> &a, const vector<ll> &m)
{
    if (a.empty())
        return {0, 1};
    ll rem = a[0], mod = m[0];
    for (size_t i = 1; i < a.size(); i++)
    {
        ll x, y;
        ll g = extgcd_crt(mod, m[i], x, y);
        if ((a[i] - rem) % g != 0)
            return {-1, -1};
        ll target = (a[i] - rem) / g;
        ll m_i_g = m[i] / g;
        x = (x % m_i_g + m_i_g) % m_i_g;
        x = (x * (target % m_i_g + m_i_g)) % m_i_g;
        rem += mod * x;
        mod *= m_i_g;
        rem = (rem % mod + mod) % mod;
    }
    return {rem, mod};
}