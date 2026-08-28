/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: ADAPTIVE SIMPSON's RULE
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Area under curve", "Evaluate integral", "Continuous probability".
 * - Classic Scenarios: You are given a mathematical function f(x) and need the
 *   integral from A to B with a precision of 10^-6.
 * - The Magic: Instead of chopping the interval into 10^6 fixed pieces (which can
 *   TLE or lose precision), Adaptive Simpson's evaluates the error dynamically.
 *   It recursively splits the interval only in areas where the curve is highly
 *   irregular, guaranteeing extreme precision in logarithmic time.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       double area = integrate(A, B, 1e-7, my_function);
 *
 * - Complexity:
 *       Time: O(Depends on curve complexity, practically instant).
 *       Space: O(Recursion depth).
 */

double simpson(double l, double r, double f_l, double f_r, double f_mid)
{
    return (f_l + 4 * f_mid + f_r) * (r - l) / 6.0;
}

double adaptive_simpson(double l, double r, double f_l, double f_r, double f_mid, double S, double eps, const function<double(double)> &f)
{
    double mid = (l + r) / 2.0;
    double l_mid = (l + mid) / 2.0;
    double r_mid = (mid + r) / 2.0;

    double f_l_mid = f(l_mid);
    double f_r_mid = f(r_mid);

    double left_S = simpson(l, mid, f_l, f_mid, f_l_mid);
    double right_S = simpson(mid, r, f_mid, f_r, f_r_mid);

    if (abs(left_S + right_S - S) <= 15.0 * eps)
    {
        return left_S + right_S + (left_S + right_S - S) / 15.0;
    }

    return adaptive_simpson(l, mid, f_l, f_mid, f_l_mid, left_S, eps / 2.0, f) +
           adaptive_simpson(mid, r, f_mid, f_r, f_r_mid, right_S, eps / 2.0, f);
}

double integrate(double a, double b, double eps, const function<double(double)> &f)
{
    double f_a = f(a);
    double f_b = f(b);
    double f_mid = f((a + b) / 2.0);
    double S = simpson(a, b, f_a, f_b, f_mid);
    return adaptive_simpson(a, b, f_a, f_b, f_mid, S, eps, f);
}