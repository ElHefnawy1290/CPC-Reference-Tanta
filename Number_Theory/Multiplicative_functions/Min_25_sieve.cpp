/**
 * 🎯 MIN-25 SIEVE
 *
 * WHAT DOES THIS DO?
 * Computes the prefix sums of a multiplicative function f(x) in sub-linear time.
 * Specifically, it calculates sum(f(i)) for 1 <= i <= N in O(N^(3/4) / log N) or O(N^(1 - eps)).
 *
 * CONDITIONS FOR f(x):
 * 1. f(p) must be a polynomial (e.g., f(p) = 1, f(p) = p, f(p) = p^2).
 * 2. f(p^c) can be calculated quickly in O(1).
 *
 * This template computes the sum of primes and count of primes as the base layer,
 * which is the starting point for 90% of Min-25 problems.
 * Modify S(x, y) to match the f(p^c) of your specific problem.
 */

using ll = long long;

struct Min25
{
    ll n, sq, m;
    vector<int> primes;
    vector<bool> is_prime;

    // Arrays for the DP state
    vector<ll> w;         // Stores the distinct values of floor(n / i)
    vector<ll> g0;        // DP table for f(p) = 1 (Count of primes)
    vector<ll> g1;        // DP table for f(p) = p (Sum of primes)
    vector<int> id1, id2; // id1 for x <= sq, id2 for x > sq

    // Constructor initializes the structures and runs the prime sieve up to sqrt(N)
    Min25(ll _n) : n(_n)
    {
        sq = sqrt(n);
        is_prime.assign(sq + 1, true);
        is_prime[0] = is_prime[1] = false;

        for (int p = 2; p <= sq; p++)
        {
            if (is_prime[p])
            {
                primes.push_back(p);
                for (int i = p * p; i <= sq; i += p)
                    is_prime[i] = false;
            }
        }

        id1.assign(sq + 1, 0);
        id2.assign(sq + 1, 0);
        m = 0;

        // Step 1: Initialize all distinct values of floor(n / i)
        for (ll i = 1, j; i <= n; i = n / j + 1)
        {
            j = n / i;
            w.push_back(j);

            // Assign DP base cases: treating all numbers as if they were prime
            // g0 = j - 1 (count of numbers from 2 to j)
            // g1 = (j * (j + 1) / 2) - 1 (sum of numbers from 2 to j)
            g0.push_back(j - 1);
            g1.push_back(j * (j + 1) / 2 - 1);

            if (j <= sq)
                id1[j] = m;
            else
                id2[n / j] = m;
            m++;
        }

        // Step 2: The DP over primes (Filtering out composites)
        for (int p : primes)
        {
            ll p2 = 1LL * p * p;
            for (int i = 0; i < m && w[i] >= p2; i++)
            {
                ll next_val = w[i] / p;
                int id = (next_val <= sq) ? id1[next_val] : id2[n / next_val];

                // Subtract the contribution of composite numbers that have 'p' as their smallest prime factor
                // g(w[i]) = g(w[i]) - f(p) * (g(w[i] / p) - sum(f(p_j)) for j < current_prime)

                // For g0: f(p) = 1
                g0[i] -= 1LL * (g0[id] - g0[id1[p - 1]]);

                // For g1: f(p) = p
                g1[i] -= 1LL * p * (g1[id] - g1[id1[p - 1]]);
            }
        }
    }

    /**
     * Step 3: Compute the actual sum of f(x)
     * @param x: Current value (n / i)
     * @param y: Index of the minimum prime factor we are considering
     */
    ll S(ll x, int y)
    {
        if (x <= 1 || primes[y] > x)
            return 0;

        int id = (x <= sq) ? id1[x] : id2[n / x];

        // Initial value: sum of f(p) for all primes in the range (primes[y], x]
        // Example for f(p) = p: ans = g1[id] - g1[id1[primes[y]]]
        // MODIFICATION: Change this formula based on your f(p)
        ll ans = (g1[id] - g1[id1[primes[y]]]);

        // Add the contribution of composite numbers
        for (int i = y; i < primes.size() && 1LL * primes[i] * primes[i] <= x; i++)
        {
            ll p = primes[i];
            ll p_pow = p;
            ll p_next = p * p;

            for (int e = 1; p_next <= x; e++, p_pow = p_next, p_next *= p)
            {
                // ans += f(p^e) * S(x / p^e, i + 1) + f(p^(e+1))
                // MODIFICATION: Change the f(p^e) multipliers here
                ans += p_pow * S(x / p_pow, i + 1) + p_next;
            }
        }
        return ans;
    }

    ll solve()
    {
        // Adding f(1) at the end, which is usually 1
        return S(n, 0) + 1;
    }
};