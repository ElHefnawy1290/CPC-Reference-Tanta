/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: DISTINCT K-TH POWERS & X^2 = 1 MOD M
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "How many perfect K-th powers modulo P", "Square roots of 1 modulo M".
 * - The Magic: These are closed-form formula generators. Calculating the number
 *   of valid values for x^2 = 1 mod M depends heavily on the prime factorization
 *   of M (specifically treating the powers of 2 differently than odd primes).
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       ll ans1 = distinct_kth_powers_prime_mod(K, P); // Requires P to be a prime
 *       ll ans2 = solutions_x2_1_mod_m(M);
 */

using ll = long long;

ll distinct_kth_powers_prime_mod(ll k, ll p)
{
    return (p - 1) / __gcd(k, p - 1) + 1;
}

ll solutions_x2_1_mod_m(ll m)
{
    if (m <= 1)
        return 1;
    ll count = 1;
    ll temp = m;
    int e2 = 0;
    while (temp % 2 == 0)
    {
        e2++;
        temp /= 2;
    }
    if (e2 == 2)
        count *= 2;
    else if (e2 >= 3)
        count *= 4;

    for (ll i = 3; i * i <= temp; i += 2)
    {
        if (temp % i == 0)
        {
            count *= 2;
            while (temp % i == 0)
                temp /= i;
        }
    }
    if (temp > 1)
        count *= 2;
    return count;
}