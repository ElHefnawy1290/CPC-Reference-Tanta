/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: LINEAR CONGRUENCE EQUATION
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Solve ax = b mod m".
 * - Classic Scenarios: You are given a, b, and m, and need to find x.
 * - The Magic: It uses the Extended Euclidean Algorithm to solve ax + my = b.
 *   It handles cases where gcd(a, m) != 1. If a solution exists, it returns
 *   the base solution x0 and the number of distinct solutions modulo m.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       ll x, num_solutions;
 *       bool possible = linear_congruence(a, b, m, x, num_solutions);
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

bool linear_congruence(ll a, ll b, ll m, ll &x, ll &num_solutions)
{
    ll x0, y0;
    ll g = extgcd(abs(a), m, x0, y0);
    if (b % g != 0)
    {
        x = 0;
        num_solutions = 0;
        return false;
    }
    x0 = (x0 % (m / g) + (m / g)) % (m / g);
    x = (x0 * (b / g)) % (m / g);
    num_solutions = g;
    return true;
}