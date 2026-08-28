/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: FAST DOUBLING FIBONACCI
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "N-th Fibonacci", "Massive N".
 * - Classic Scenarios: You need F_N modulo M where N = 10^18.
 * - The Magic: Derives F_{2k} and F_{2k+1} directly from F_k and F_{k+1}.
 *   It completely sidesteps matrix overhead, making it drastically faster
 *   and much shorter to type than a robust matrix exponentiation struct.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       pair<ll, ll> ans = fast_doubling(N, M);
 * - Result Breakdown:
 *       ans.first = F_N
 *       ans.second = F_{N+1}
 *
 * - Complexity:
 *       Time: Strictly O(log N).
 *       Space: O(log N) for the recursion stack.
 */

using ll = long long;

pair<ll, ll> fast_doubling(ll n, ll mod)
{
    if (n == 0)
        return {0, 1};
    auto p = fast_doubling(n >> 1, mod);
    ll c = p.first * (2 * p.second - p.first + mod) % mod;
    ll d = (p.first * p.first + p.second * p.second) % mod;
    if (n & 1)
        return {d, (c + d) % mod};
    return {c, d};
}