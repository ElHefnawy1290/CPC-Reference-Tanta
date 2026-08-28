/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: SEGMENTED SIEVE
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Primes in Range [L, R]", "Massive Bounds".
 * - Classic Scenarios: Find all primes or the number of primes between L and R.
 *   L and R can be as massive as 10^12, but the difference (R - L) is small (<= 10^6).
 * - The Magic: A regular sieve cannot allocate an array up to 10^12. The segmented
 *   sieve only sieves up to sqrt(R), caches those base primes, and then "shifts"
 *   the sieving logic directly to the target window [L, R], ignoring the void before L.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       vector<uint64_t> primes = segmented_sieve(L, R);
 *
 * - Complexity:
 *       Time: O((R - L + sqrt(R)) * log(log(R)))
 *       Space: O(sqrt(R) + (R - L))
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Edge Cases Guarded: Safely handles constraints where L = 0 or L = 1
 *   (which are not prime but often trip up basic segmented implementations).
 */

using u64 = uint64_t;

vector<u64> segmented_sieve(u64 L, u64 R)
{
    if (L > R)
        return {};
    u64 lim = sqrt(R);
    vector<bool> mark(lim + 1, false);
    vector<u64> primes;
    for (u64 i = 2; i <= lim; ++i)
    {
        if (!mark[i])
        {
            primes.push_back(i);
            for (u64 j = i * i; j <= lim; j += i)
                mark[j] = true;
        }
    }

    vector<bool> isPrime(R - L + 1, true);
    for (u64 p : primes)
    {
        u64 start = max(p * p, (L + p - 1) / p * p);
        for (u64 j = start; j <= R; j += p)
            isPrime[j - L] = false;
    }

    if (L == 0)
    {
        isPrime[0] = false;
        if (R >= 1)
            isPrime[1] = false;
    }
    else if (L == 1)
    {
        isPrime[0] = false;
    }

    vector<u64> range_primes;
    for (u64 i = 0; i <= R - L; ++i)
    {
        if (isPrime[i])
            range_primes.push_back(L + i);
    }
    return range_primes;
}