/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: STERN-BROCOT TREE (SIMPLEST FRACTION)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Stern-Brocot", "Simplest Fraction", "Rational Approximation".
 * - Classic Scenarios: Find the fraction P/Q with the absolute smallest
 *   denominator Q that strictly lies between two other fractions A/B and C/D.
 * - The Magic: Instead of simulating the binary search descent down the
 *   Stern-Brocot tree (which can TLE in O(max(P, Q))), it uses a continued
 *   fraction / Euclidean acceleration to jump down the branches.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       pair<long long, long long> ans = simplest_fraction(A, B, C, D);
 *       // Returns {P, Q} such that A/B < P/Q < C/D.
 *
 * - Complexity:
 *       Time: O(log(max(A, B, C, D)))
 *       Space: O(log(max)) for recursion.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Fraction format: Ensure your input fractions A/B and C/D are simplified
 *   and that A/B is strictly less than C/D before passing them in.
 */

using ll = long long;

pair<ll, ll> simplest_fraction(ll a, ll b, ll c, ll d)
{
    if (a / b < c / d)
        return {a / b + 1, 1};
    if (a == 0)
        return {1, d / c + 1};
    auto res = simplest_fraction(d, c % d, b, a % b);
    return {res.second + (a / b) * res.first, res.first};
}