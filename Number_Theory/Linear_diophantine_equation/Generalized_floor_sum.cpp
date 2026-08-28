/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: GENERALIZED FLOOR SUM
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Sum of i * floor", "Sum of squares of floors".
 * - Classic Scenarios: A step above standard floor sum, used when evaluating
 *   complex grid summations or polynomial constraints under a modulo line.
 * - The Magic: Through a complex series of algebraic expansions and Euclidean
 *   domain folding, it simultaneously resolves three interconnected sums in
 *   pure logarithmic time.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       FSR ans = gen_floor_sum(n, a, b, c);
 * - Result Breakdown:
 *       ans.f = sum_{i=0}^n floor((a*i+b)/c)
 *       ans.g = sum_{i=0}^n i * floor((a*i+b)/c)
 *       ans.h = sum_{i=0}^n floor((a*i+b)/c)^2
 *
 * - Complexity:
 *       Time: O(log(min(a, c)))
 *       Space: O(log(min(a, c)))
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Inclusive Bounds: This specifically calculates for i = 0 to N (inclusive).
 *   If your problem is 1-based, evaluate for N and subtract the i=0 term, or
 *   use N-1 if the upper bound is exclusive.
 */

using ll = long long;

struct FSR
{
    ll f, g, h;
};

FSR gen_floor_sum(ll n, ll a, ll b, ll c)
{
    ll ac = a / c, bc = b / c;
    if (a == 0)
    {
        ll f = (n + 1) * bc;
        ll g = bc * n * (n + 1) / 2;
        ll h = (n + 1) * bc * bc;
        return {f, g, h};
    }
    if (a >= c || b >= c)
    {
        FSR res = gen_floor_sum(n, a % c, b % c, c);
        ll f = res.f + n * (n + 1) / 2 * ac + (n + 1) * bc;
        ll g = res.g + ac * n * (n + 1) * (2 * n + 1) / 6 + bc * n * (n + 1) / 2;
        ll h = res.h + ac * ac * n * (n + 1) * (2 * n + 1) / 6 + bc * bc * (n + 1) + 2 * ac * bc * n * (n + 1) / 2 + 2 * ac * res.g + 2 * bc * res.f;
        return {f, g, h};
    }
    ll m = (a * n + b) / c;
    FSR res = gen_floor_sum(m - 1, c, c - b - 1, a);
    ll f = n * m - res.f;
    ll g = (m * n * (n + 1) - res.f - res.h) / 2;
    ll h = n * m * m - 2 * res.g - res.f;
    return {f, g, h};
}