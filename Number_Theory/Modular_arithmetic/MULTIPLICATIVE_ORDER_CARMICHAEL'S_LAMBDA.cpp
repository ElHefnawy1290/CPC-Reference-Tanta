/**
 * 🎯 MULTIPLICATIVE ORDER & CARMICHAEL'S LAMBDA
 *
 * WHAT DOES THIS DO?
 * Finds the smallest integer k such that (a^k) % n == 1.
 * Using Euler's Totient phi(n) is too slow to find the exact order because
 * the order divides Carmichael's Lambda function, which divides phi(n).
 *
 * MATH BACKGROUND:
 * lambda(p^k) = p^(k-1) * (p - 1)  [except for p=2, k>=3 where it's divided by 2]
 * lambda(a * b) = lcm(lambda(a), lambda(b))
 * The multiplicative order of 'a' modulo 'n' strictly divides lambda(n).
 *
 * Complexity: O(sqrt(N)) to factorize N.
 */

using ll = long long;

ll gcd(ll a, ll b)
{
    while (b)
    {
        a %= b;
        swap(a, b);
    }
    return a;
}

ll lcm(ll a, ll b)
{
    return (a / gcd(a, b)) * b;
}

// Modular exponentiation using __int128 to prevent overflow when multiplying two long longs
ll power(ll base, ll exp, ll mod)
{
    ll res = 1;
    base %= mod;
    while (exp > 0)
    {
        if (exp % 2 == 1)
            res = (ll)((unsigned __int128)res * base % mod);
        base = (ll)((unsigned __int128)base * base % mod);
        exp /= 2;
    }
    return res;
}

ll carmichael_lambda(ll n)
{
    ll lambda = 1;
    ll temp = n;

    // Step 1: Prime factorization of n
    for (ll i = 2; i * i <= temp; i++)
    {
        if (temp % i == 0)
        {
            ll p_pow = 1;
            while (temp % i == 0)
            {
                p_pow *= i;
                temp /= i;
            }

            // term = phi(p^k) = p^k - p^(k-1)
            ll term = p_pow - p_pow / i;

            // Special case for Carmichael: if p = 2 and k >= 3, lambda is phi/2
            if (i == 2 && p_pow >= 8)
            {
                term /= 2;
            }
            lambda = lcm(lambda, term);
        }
    }

    // If there is a prime factor greater than sqrt(n)
    if (temp > 1)
    {
        lambda = lcm(lambda, temp - 1);
    }

    return lambda;
}

ll multiplicative_order(ll a, ll n)
{
    // If a and n are not coprime, order doesn't exist
    if (gcd(a, n) != 1)
        return -1;

    ll lambda = carmichael_lambda(n);
    ll order = lambda;
    ll temp_lambda = lambda;

    // Step 2: Find the exact order by removing prime factors from lambda
    // and checking if it still satisfies a^order % n == 1
    for (ll i = 2; i * i <= temp_lambda; i++)
    {
        if (temp_lambda % i == 0)
        {
            while (temp_lambda % i == 0)
            {
                temp_lambda /= i;
            }
            // Try dividing the order by the prime factor 'i'
            while (order % i == 0 && power(a, order / i, n) == 1)
            {
                order /= i;
            }
        }
    }

    // Check the remaining prime factor
    if (temp_lambda > 1)
    {
        while (order % temp_lambda == 0 && power(a, order / temp_lambda, n) == 1)
        {
            order /= temp_lambda;
        }
    }

    return order;
}