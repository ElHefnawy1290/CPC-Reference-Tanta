/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: PELL'S EQUATION
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "x^2 - d*y^2 = 1", "Fundamental Solution".
 * - Classic Scenarios: Find the smallest positive integer solution (x, y)
 *   satisfying Pell's equation where d is a non-square positive integer.
 * - The Magic: It generates the continued fraction expansion of sqrt(d).
 *   The convergents (h_i / k_i) of this fraction are guaranteed to eventually
 *   satisfy the equation.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       pair<__int128, __int128> ans = solve_pell(d);
 *       // Returns {x, y}. If d is a perfect square, returns {-1, -1}.
 *
 * - Complexity:
 *       Time: O(sqrt(d) * log(d))
 *       Space: O(1)
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - 128-bit Return Type: Fundamental solutions to Pell's equation grow
 *   exponentially fast. Even for small d (e.g., d = 61), x and y can easily
 *   overflow 64-bit integers. __int128 is highly recommended here.
 * - Generating More Solutions: If (x1, y1) is the fundamental solution,
 *   the next solution (x_k, y_k) is (x1 + y1*sqrt(d))^k.
 */

using u128 = __int128;
using ll = long long;

pair<u128, u128> solve_pell(ll d)
{
    ll sq = sqrt(d);
    if (sq * sq == d)
        return {-1, -1};

    ll m = 0, d_den = 1, a = sq;
    u128 num1 = 1, num0 = a;
    u128 den1 = 0, den0 = 1;

    while (num0 * num0 - (u128)d * den0 * den0 != 1)
    {
        m = d_den * a - m;
        d_den = (d - m * m) / d_den;
        a = (sq + m) / d_den;

        u128 num2 = num1;
        num1 = num0;
        num0 = (u128)a * num1 + num2;

        u128 den2 = den1;
        den1 = den0;
        den0 = (u128)a * den1 + den2;
    }
    return {num0, den0};
}