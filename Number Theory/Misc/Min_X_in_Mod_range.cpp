/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: MINIMUM X IN MODULO RANGE
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Smallest x", "L <= ax mod m <= R", "Min of Mod of AP".
 * - Classic Scenarios: Finding the first index where an arithmetic progression
 *   wraps around a modulo into a specific target window.
 * - The Magic: It uses a recursive Euclidean-style domain folding. Instead
 *   of simulating the progression, it inverts the bounds and steps to find
 *   the exact minimum x in O(log M) time.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       long long x = min_x_in_mod_range(A, M, L, R);
 *       // Returns -1 if no such x exists.
 *
 * - Complexity:
 *       Time: O(log M)
 *       Space: O(log M) for recursion.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Min of Mod of AP (Topic 295): If you just need the absolute minimum of
 *   (A*x) mod M for x in [0, N], you can binary search the answer V in [0, M-1]
 *   and use this function to check if a valid x <= N exists for L=0, R=V.
 */

using ll = long long;

ll min_x_in_mod_range(ll a, ll m, ll l, ll r)
{
    if (l == 0)
        return 0;
    if (a == 0)
        return -1;
    ll step = (l + a - 1) / a;
    if (step * a <= r)
        return step;
    ll b = m % a;
    ll res = min_x_in_mod_range(b, a, a - r % a, a - l % a);
    if (res == -1)
        return -1;
    return (l + res * b + a - 1) / a;
}