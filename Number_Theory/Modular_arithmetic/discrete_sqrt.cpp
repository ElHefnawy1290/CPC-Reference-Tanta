/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: TONELLI-SHANKS ALGORITHM
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Quadratic Residue", "Modular Square Root", "x^2 = N mod P".
 * - Classic Scenarios: You are given an integer N and an odd prime P, and you
 *   need to find x such that x^2 ≡ N (mod P).
 * - The Magic: It factors P-1 as Q * 2^S. By finding a quadratic non-residue Z,
 *   it iteratively manipulates the exponents to isolate the square root. It is
 *   a beautiful but mathematically dense algorithm that you do not want to debug live.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       long long root = tonelli_shanks(N, P);
 *       // Returns -1 if no square root exists.
 *       // If 'root' is a solution, 'P - root' is the other valid solution.
 *
 * - Complexity:
 *       Time: O(log^2 P) worst case, O(log P) on average. Extremely fast.
 *       Space: O(1)
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Prime Modulo Only: This specifically requires P to be an odd prime. If you
 *   need to find roots modulo a composite number, you must use Tonelli-Shanks
 *   on its prime factors and combine the results using the Chinese Remainder Theorem (CRT).
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

ll tonelli_shanks(ll n, ll p)
{
    if (n == 0)
        return 0;
    if (custom_pow(n, (p - 1) / 2, p) != 1)
        return -1;

    ll q = p - 1, s = 0;
    while (q % 2 == 0)
    {
        s++;
        q /= 2;
    }

    if (s == 1)
        return custom_pow(n, (p + 1) / 4, p);

    ll z = 2;
    while (custom_pow(z, (p - 1) / 2, p) != p - 1)
    {
        z++;
    }

    ll c = custom_pow(z, q, p);
    ll r = custom_pow(n, (q + 1) / 2, p);
    ll t = custom_pow(n, q, p);
    ll m = s;

    while (t != 1)
    {
        ll t2 = (t * t) % p;
        ll i = 1;
        for (; i < m; i++)
        {
            if (t2 == 1)
                break;
            t2 = (t2 * t2) % p;
        }

        ll b = c;
        for (int j = 0; j < m - i - 1; j++)
        {
            b = (b * b) % p;
        }

        r = (r * b) % p;
        c = (b * b) % p;
        t = (t * c) % p;
        m = i;
    }
    return r;
}