/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: FRACTION BINARY SEARCH
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Best rational approximation", "Fraction with bounded denominator".
 * - Classic Scenarios: You are given a target floating point value and must find
 *   a fraction P/Q exactly equal to it (or closest to it) such that Q <= MAX_D.
 * - The Magic: The Stern-Brocot tree generates all positive rational numbers in
 *   simplest form. Given bounds a/b and c/d, the mediant (a+c)/(b+d) is strictly
 *   between them. We binary search down this tree.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       pair<ll, ll> best_fraction = fraction_search(target, MAX_D);
 *
 * - Complexity:
 *       Time: O(log(MAX_D))
 */

using ll = long long;

// Returns {numerator, denominator}
pair<ll, ll> fraction_search(double target, ll max_d)
{
    ll a = 0, b = 1; // Lower bound: 0/1
    ll c = 1, d = 0; // Upper bound: 1/0 (Infinity)

    ll best_num = 0, best_den = 1;
    double min_diff = 1e18;

    while (b + d <= max_d)
    {
        ll mid_num = a + c;
        ll mid_den = b + d;

        double mid_val = (double)mid_num / mid_den;
        double diff = abs(mid_val - target);

        if (diff < min_diff)
        {
            min_diff = diff;
            best_num = mid_num;
            best_den = mid_den;
        }

        if (mid_val == target)
        {
            return {mid_num, mid_den};
        }
        else if (mid_val < target)
        {
            a = mid_num;
            b = mid_den;
        }
        else
        {
            c = mid_num;
            d = mid_den;
        }
    }

    return {best_num, best_den};
}