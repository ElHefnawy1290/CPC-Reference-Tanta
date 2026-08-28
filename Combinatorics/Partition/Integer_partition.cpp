/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: INTEGER PARTITION
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Ways to sum to N", "Unordered integer partitions".
 * - Classic Scenarios: Find the number of distinct ways to represent the integer N
 *   as a sum of positive integers (e.g., 4 = 4, 3+1, 2+2, 2+1+1, 1+1+1+1).
 * - The Magic: A standard coin-change DP takes O(N^2), which crashes for N = 10^5.
 *   Euler's Pentagonal Number Theorem computes the partition function P(N) in O(N sqrt(N))
 *   by subtracting and adding previous P() values at specifically spaced pentagonal
 *   intervals: P(k) = P(k-1) + P(k-2) - P(k-5) - P(k-7) + P(k-12) + ...
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       vector<long long> P = partition_function(N, MOD);
 *       // P[i] contains the number of partitions for integer i.
 *
 * - Complexity:
 *       Time: O(N * sqrt(N))
 *       Space: O(N)
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Array Precomputation: It returns the entire array up to N. If the problem has
 *   multiple test cases querying different N's, run this function ONCE globally for
 *   the maximum possible N, then answer each query in O(1).
 */

using ll = long long;

vector<ll> partition_function(int n, ll mod)
{
    vector<ll> p(n + 1, 0);
    p[0] = 1;
    for (int i = 1; i <= n; i++)
    {
        for (int k = 1;; k++)
        {
            int p1 = k * (3 * k - 1) / 2;
            int p2 = k * (3 * k + 1) / 2;
            if (p1 > i && p2 > i)
                break;

            ll sign = (k % 2 == 1) ? 1 : -1;

            if (p1 <= i)
                p[i] = (p[i] + sign * p[i - p1]) % mod;
            if (p2 <= i)
                p[i] = (p[i] + sign * p[i - p2]) % mod;
        }
        p[i] = (p[i] + mod) % mod;
    }
    return p;
}