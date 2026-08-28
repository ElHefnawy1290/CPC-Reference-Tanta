/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: BOUNDED LDE2
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Solutions in range", "ax + by = c", "Bounded variables".
 * - Classic Scenarios: You need the exact count of integer pairs (x, y) satisfying
 *   ax + by = c such that min_x <= x <= max_x and min_y <= y <= max_y.
 * - The Magic: It finds a base solution using Extended Euclidean Algorithm, then
 *   mathematically shifts the solution using (b/g) and (a/g) to find the absolute
 *   minimum and maximum valid boundaries, intersecting them safely.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       long long total = count_solutions(a, b, c, min_x, max_x, min_y, max_y);
 *
 * - Complexity:
 *       Time: O(log(min(a, b)))
 *       Space: O(1)
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Zero Handling: Safely handles edge cases where a = 0 or b = 0, which often
 *   crash naive implementations.
 */

using ll = long long;

ll extgcd(ll a, ll b, ll &x, ll &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }
    ll x1, y1;
    ll d = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

void shift_solution(ll &x, ll &y, ll a, ll b, ll cnt)
{
    x += cnt * b;
    y -= cnt * a;
}

ll count_solutions(ll a, ll b, ll c, ll min_x, ll max_x, ll min_y, ll max_y)
{
    if (a == 0 && b == 0)
    {
        if (c == 0)
            return (max_x - min_x + 1) * (max_y - min_y + 1);
        return 0;
    }
    if (a == 0)
    {
        if (c % b == 0 && c / b >= min_y && c / b <= max_y)
            return max_x - min_x + 1;
        return 0;
    }
    if (b == 0)
    {
        if (c % a == 0 && c / a >= min_x && c / a <= max_x)
            return max_y - min_y + 1;
        return 0;
    }

    ll x, y;
    ll g = extgcd(abs(a), abs(b), x, y);
    if (c % g != 0)
        return 0;

    x *= c / g;
    y *= c / g;
    if (a < 0)
        x = -x;
    if (b < 0)
        y = -y;

    ll a_g = a / g, b_g = b / g;
    ll sign_a = a > 0 ? +1 : -1;
    ll sign_b = b > 0 ? +1 : -1;

    shift_solution(x, y, a_g, b_g, (min_x - x) / b_g);
    if (x < min_x)
        shift_solution(x, y, a_g, b_g, sign_b);
    if (x > max_x)
        return 0;
    ll lx1 = x;

    shift_solution(x, y, a_g, b_g, (max_x - x) / b_g);
    if (x > max_x)
        shift_solution(x, y, a_g, b_g, -sign_b);
    ll rx1 = x;

    shift_solution(x, y, a_g, b_g, -(min_y - y) / a_g);
    if (y < min_y)
        shift_solution(x, y, a_g, b_g, -sign_a);
    if (y > max_y)
        return 0;
    ll lx2 = x;

    shift_solution(x, y, a_g, b_g, -(max_y - y) / a_g);
    if (y > max_y)
        shift_solution(x, y, a_g, b_g, sign_a);
    ll rx2 = x;

    if (lx2 > rx2)
        swap(lx2, rx2);
    ll lx = max(lx1, lx2);
    ll rx = min(rx1, rx2);

    if (lx > rx)
        return 0;
    return (rx - lx) / abs(b_g) + 1;
}