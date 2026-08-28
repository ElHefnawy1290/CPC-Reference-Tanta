/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: FAST DIVISOR COUNTING (WHEEL FACTORIZATION)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Number of divisors of N", "N up to 10^18", "Count divisors without full factorization".
 * - Classic Scenarios: You are given a massive number N (up to 10^14 or 10^18) and you need to
 *   find exactly how many divisors it has, but O(sqrt(N)) is too slow, and you can't build a Sieve
 *   because N is too large.
 * - The Magic: "Wheel Factorization". Instead of checking every number up to sqrt(N), we manually
 *   factor out the first 8 primes (2, 3, 5, 7, 11, 13, 17, 19). The product of these primes is ~9.6 million.
 *   For the remaining part of N, we ONLY check divisors that are coprime to these 8 primes.
 *   This eliminates about ~83% of the useless checks in the O(sqrt(N)) loop!
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Create an instance globally or in `main`. It will precompute the "wheel spokes" ONCE.
 *       FastDivisorCounter fdc;
 *
 * - Queries: Pass your number N (up to 10^18) to get the number of divisors.
 *       ll total_divisors = fdc.count(N);
 *
 * - Complexity:
 *       Time (Initialization): Precomputing the wheel takes O(P) where P is ~9.6 * 10^6 (Done once in ~15ms).
 *       Time (Per Query): roughly O(sqrt(N) / 9.6*10^6). Lightning fast for huge queries!
 *       Space: ~6 MB for the precomputed wheel.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - If Memory Limit is EXTREMELY tight (<10 MB), you can remove the `spokes` vector and calculate the
 *   coprime numbers inside the `count` function loop like your original code, but only if Q=1.
 *   For Q > 1, the precomputed `spokes` vector is vastly superior.
 */

#include <bits/stdc++.h>
using namespace std;
#define ll long long
struct FastDivisorCounter
{
private:
    vector<int> primes;
    ll wheel_size;
    vector<ll> spokes; // Stores numbers from 1 to wheel_size that are coprime to the base primes
public:
    FastDivisorCounter()
    {
        primes = {2, 3, 5, 7, 11, 13, 17, 19};
        wheel_size = 1;

        for (int p : primes)
        {
            wheel_size *= p;
        }
        // ? PRECOMPUTATION MAGIC: We find all valid "spokes" ONCE.
        // There are exactly 1,658,880 such numbers. By saving them, we skip millions
        // of useless iterations in future queries.
        for (ll i = 1; i < wheel_size; i++)
        {
            bool is_coprime = true;
            for (int p : primes)
            {
                if (i % p == 0)
                {
                    is_coprime = false;
                    break;
                }
            }
            if (is_coprime)
            {
                spokes.push_back(i);
            }
        }
    }
    ll count(ll n)
    {
        ll ans = 1;
        // Step 1: Remove all factors of the base primes and count their contribution
        for (int p : primes)
        {
            int c = 0;
            while (n % p == 0)
            {
                n /= p;
                c++;
            }
            // Standard divisor formula: multiply by (power + 1)
            ans *= (c + 1);
        }
        // Step 2: The remaining part of 'n' is now completely coprime to our base primes.
        // We only check potential divisors using our precomputed wheel spokes.
        ll all = 0;
        for (ll spoke : spokes)
        {
            ll o = spoke;
            // ? THE SPEED JUMP: We jump by `wheel_size` (9,699,690) instead of +1 or +2!
            for (; o * o < n; o += wheel_size)
            {
                if (n % o == 0)
                {
                    all += 2; // Found pair of divisors: `o` and `n/o`
                }
            }

            // Check perfect square
            if (o * o == n)
            {
                all++;
            }
        }
        // The total divisors of the original N is the product of the divisors
        // of the base-prime part and the divisors of the remaining coprime part.
        return ans * all;
    }
};
void solve()
{
    int t;
    if (!(cin >> t))
        return;
    // Initialize OUTSIDE the query loop so the wheel is built only once!
    FastDivisorCounter fdc;
    while (t--)
    {
        ll n;
        cin >> n;
        cout << fdc.count(n) << "\n";
    }
}
