/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: MILLER-RABIN & POLLARD'S RHO
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Primality Test", "Prime Factorization", "Count Divisors", "Massive N".
 * - Classic Scenarios: You need to check if N is prime, find all prime factors of N,
 *   or count the total number of divisors where N is up to 10^18. Standard O(sqrt(N))
 *   trial division will result in a Time Limit Exceeded (TLE) verdict.
 * - The Magic: Miller-Rabin deterministically checks primality for 64-bit integers
 *   in O(log N) time using 7 specific mathematical bases. If it's composite, Pollard's Rho
 *   steps in to find a prime factor in expected O(N^(1/4)) time. Together, they instantly
 *   break down 10^18 numbers.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Primality Check:
 *       bool is_prime = MillerRabin(n);
 * - Prime Factorization:
 *       map<uint64_t, int> prime_factors;
 *       factorize(n, prime_factors); // Populates the map with {prime, frequency}
 * - Count Total Divisors:
 *       uint64_t total = count_divisors(n);
 *
 * - Complexity:
 *       Time: O(log N) for Miller-Rabin. Expected O(N^(1/4)) for Pollard's Rho.
 *       Space: O(log N) for recursion depth and the frequency map.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Type Aliases: This template strictly uses `u64` (uint64_t) and `u128` (__uint128_t)
 *   to prevent silent overflows during modular multiplication of massive numbers.
 *   Do not change these to standard `long long`.
 * - Randomness: `rand()` is used for the Rho polynomial `x^2 + c`. This is usually
 *   sufficient, but if you hit a malicious anti-hash test case, you can swap it
 *   for a faster, custom `mt19937_64` implementation.
 */

using u64 = uint64_t;
using u128 = __uint128_t;

u64 binpower(u64 base, u64 e, u64 mod)
{
    u64 result = 1;
    base %= mod;
    while (e)
    {
        if (e & 1)
            result = (u128)result * base % mod;
        base = (u128)base * base % mod;
        e >>= 1;
    }
    return result;
}

bool check_composite(u64 n, u64 a, u64 d, int s)
{
    u64 x = binpower(a, d, n);
    if (x == 1 || x == n - 1)
        return false;
    for (int r = 1; r < s; r++)
    {
        x = (u128)x * x % n;
        if (x == n - 1)
            return false;
    }
    return true;
}

bool MillerRabin(u64 n)
{
    if (n < 2)
        return false;
    int r = 0;
    u64 d = n - 1;
    while ((d & 1) == 0)
    {
        d >>= 1;
        r++;
    }
    // These 7 bases are deterministically sufficient for all n < 2^64
    for (u64 a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022})
    {
        if (n == a)
            return true;
        if (check_composite(n, a, d, r))
            return false;
    }
    return true;
}

u64 pollard_rho(u64 n)
{
    if (n % 2 == 0)
        return 2;
    if (MillerRabin(n))
        return n;

    u64 x = 2, y = 2, d = 1, c = 1;
    auto f = [&](u64 x, u64 n, u64 c)
    {
        return (u64)(((u128)x * x + c) % n);
    };

    while (d == 1)
    {
        x = f(x, n, c);
        y = f(f(y, n, c), n, c);
        d = __gcd(x > y ? x - y : y - x, n);
        if (d == n)
        {
            x = rand() % (n - 2) + 2;
            y = x;
            c = rand() % (n - 1) + 1;
            d = 1;
        }
    }
    return d;
}

void factorize(u64 n, map<u64, int> &prime_factors)
{
    if (n == 1)
        return;
    if (MillerRabin(n))
    {
        prime_factors[n]++;
        return;
    }
    u64 divisor = pollard_rho(n);
    factorize(divisor, prime_factors);
    factorize(n / divisor, prime_factors);
}

u64 count_divisors(u64 n)
{
    map<u64, int> prime_factors;
    factorize(n, prime_factors);
    u64 total_divisors = 1;
    for (auto &pf : prime_factors)
    {
        total_divisors *= (pf.second + 1);
    }
    return total_divisors;
}