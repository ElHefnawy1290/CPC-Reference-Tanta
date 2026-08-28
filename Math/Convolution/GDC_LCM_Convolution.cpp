/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: GCD & LCM CONVOLUTION
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "gcd(i, j) = k", "lcm(i, j) = k", "Divisor transformation".
 * - Classic Scenarios: Find array C where C[k] = sum(A[i] * B[j]) over all pairs
 *   (i, j) such that gcd(i, j) == k (or lcm(i, j) == k).
 * - The Magic: Bypasses polynomial rings entirely. It transforms the arrays into
 *   prefix sums over their multiples (for GCD) or divisors (for LCM), multiplies
 *   the point values, and reverses the transformation using Mobius-style subtraction.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       vector<long long> C_gcd = gcd_convolution(A, B);
 *       vector<long long> C_lcm = lcm_convolution(A, B);
 *
 * - Complexity:
 *       Time: O(N log log N) utilizing a prime sieve optimization.
 *       Space: O(N).
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - 1-Based Indexing: These arrays MUST be 1-indexed (i.e., A[1] is the first
 *   element, A[0] is ignored). Ensure A and B are the same size before passing.
 */

using ll = long long;
const ll MOD = 998244353;

vector<int> get_primes(int n)
{
    vector<bool> is_prime(n + 1, true);
    vector<int> primes;
    for (int p = 2; p <= n; p++)
    {
        if (is_prime[p])
        {
            primes.push_back(p);
            for (int i = p * 2; i <= n; i += p)
                is_prime[i] = false;
        }
    }
    return primes;
}

vector<ll> gcd_convolution(vector<ll> a, vector<ll> b)
{
    int n = a.size() - 1;
    vector<int> primes = get_primes(n);

    for (int p : primes)
    {
        for (int i = n / p; i > 0; i--)
        {
            a[i] = (a[i] + a[i * p]) % MOD;
            b[i] = (b[i] + b[i * p]) % MOD;
        }
    }

    vector<ll> c(n + 1);
    for (int i = 1; i <= n; i++)
    {
        c[i] = a[i] * b[i] % MOD;
    }

    for (int p : primes)
    {
        for (int i = 1; i * p <= n; i++)
        {
            c[i] = (c[i] - c[i * p] + MOD) % MOD;
        }
    }
    return c;
}

vector<ll> lcm_convolution(vector<ll> a, vector<ll> b)
{
    int n = a.size() - 1;
    vector<int> primes = get_primes(n);

    for (int p : primes)
    {
        for (int i = 1; i * p <= n; i++)
        {
            a[i * p] = (a[i * p] + a[i]) % MOD;
            b[i * p] = (b[i * p] + b[i]) % MOD;
        }
    }

    vector<ll> c(n + 1);
    for (int i = 1; i <= n; i++)
    {
        c[i] = a[i] * b[i] % MOD;
    }

    for (int p : primes)
    {
        for (int i = n / p; i > 0; i--)
        {
            c[i * p] = (c[i * p] - c[i] + MOD) % MOD;
        }
    }
    return c;
}