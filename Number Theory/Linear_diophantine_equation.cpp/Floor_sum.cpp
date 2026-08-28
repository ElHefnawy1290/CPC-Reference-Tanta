/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: FLOOR SUM
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "ax + by <= c", "Lattice points under line", "Sum of arithmetic progression mod M".
 * - Classic Scenarios: Compute sum_{i=0}^{n-1} floor((a * i + b) / m).
 * - The Magic: It operates exactly like the Euclidean algorithm, trading dimensions
 *   and folding the geometry to evaluate the entire sum logarithmically.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       long long ans = floor_sum(n, m, a, b);
 *
 * - Complexity:
 *       Time: O(log(min(m, a)))
 *       Space: O(log(min(m, a))) due to recursion.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Equivalent to ax + by <= c: For a line equation in the first quadrant, rearrange
 *   it to y <= (c - ax) / b, which directly translates to summing floor((c - a*x) / b)
 *   over all valid x.
 */

using ll = long long;

ll floor_sum(ll n, ll m, ll a, ll b)
{
    ll ans = 0;
    if (a >= m)
    {
        ans += (n - 1) * n / 2 * (a / m);
        a %= m;
    }
    if (b >= m)
    {
        ans += n * (b / m);
        b %= m;
    }
    ll y_max = (a * n + b) / m;
    ll x_max = y_max * m - b;
    if (y_max == 0)
        return ans;
    ans += (n - (x_max + a - 1) / a) * y_max;
    ans += floor_sum(y_max, a, m, (a - x_max % a) % a);
    return ans;
}