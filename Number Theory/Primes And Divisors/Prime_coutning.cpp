/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: PRIME COUNTING FUNCTION (LUCY-HEDGEHOG)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Count primes up to N", "Pi(x)", "Lucy-Hedgehog", "Min-25 Sieve".
 * - Classic Scenarios: You are given N up to 10^11 and asked exactly how many
 *   prime numbers are less than or equal to N.
 * - The Magic: A standard sieve up to 10^11 will instantly crash with Memory Limit
 *   Exceeded and Time Limit Exceeded. The Lucy-Hedgehog dynamic programming approach
 *   computes this mathematically without iterating over or storing the actual primes!
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       uint64_t total_primes = prime_count(N);
 *
 * - Complexity:
 *       Time: O(N^(3/4)) — Runs almost instantly for N = 10^10, and easily
 *       passes 1-second limits for N = 10^11.
 *       Space: O(sqrt(N)) — Extremely memory-efficient.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Array Sizes: It only creates vectors of size sqrt(N). For N = 10^11, this is
 *   just an array of size 3.16 * 10^5, taking mere megabytes of RAM.
 */

using u64 = uint64_t;

u64 prime_count(u64 N)
{
    if (N <= 1)
        return 0;
    u64 v = sqrt(N);
    vector<u64> S0(v + 1), S1(v + 1);
    for (u64 i = 1; i <= v; i++)
    {
        S0[i] = i - 1;
        S1[i] = N / i - 1;
    }
    for (u64 p = 2; p <= v; p++)
    {
        if (S0[p] == S0[p - 1])
            continue;
        u64 p_cnt = S0[p - 1];
        u64 q = p * p;
        u64 end = min(v, N / q);
        for (u64 i = 1; i <= end; i++)
        {
            u64 d = i * p;
            if (d <= v)
                S1[i] -= S1[d] - p_cnt;
            else
                S1[i] -= S0[N / d] - p_cnt;
        }
        for (u64 i = v; i >= q; i--)
        {
            S0[i] -= S0[i / p] - p_cnt;
        }
    }
    return S1[1];
}