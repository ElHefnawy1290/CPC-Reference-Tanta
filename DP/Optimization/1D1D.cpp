/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: 1D1D OPTIMIZATION (LINE CONTAINER)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Convex Hull Trick", "y = mx + c", "Li Chao Tree alternative".
 * - Classic Scenarios: dp[i] = min_{j < i} (dp[j] + M[j] * X[i] + C[j]).
 * - The Magic: Maintains the upper/lower envelope of a set of lines. It supports
 *   adding lines dynamically and querying the extreme value at any X in O(log N).
 *   This specific implementation (KACTL's LineContainer) handles lines added with
 *   NON-MONOTONIC slopes, making it universally safe.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization:
 *       LineContainer cht;
 * - Operations:
 *       cht.add(m, c);      // Adds line y = mx + c
 *       ll max_y = cht.query(x);
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Minimization: This natively computes the MAXIMUM value at x. If you need the
 *   MINIMUM value, insert lines with inverted signs: `cht.add(-m, -c)` and negate
 *   the queried result: `-cht.query(x)`.
 */

using ll = long long;

struct Line
{
    mutable ll k, m, p;
    bool operator<(const Line &o) const { return k < o.k; }
    bool operator<(ll x) const { return p < x; }
};

struct LineContainer : multiset<Line, less<>>
{
    static const ll inf = LLONG_MAX;
    ll div(ll a, ll b)
    {
        return a / b - ((a ^ b) < 0 && a % b);
    }
    bool isect(iterator x, iterator y)
    {
        if (y == end())
            return x->p = inf, 0;
        if (x->k == y->k)
            x->p = x->m > y->m ? inf : -inf;
        else
            x->p = div(y->m - x->m, x->k - y->k);
        return x->p >= y->p;
    }
    void add(ll k, ll m)
    {
        auto z = insert({k, m, 0}), y = z++, x = y;
        while (isect(y, z))
            z = erase(z);
        if (x != begin() && isect(--x, y))
            isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p)
            isect(x, erase(y));
    }
    ll query(ll x)
    {
        assert(!empty());
        auto l = *lower_bound(x);
        return l.k * x + l.m;
    }
};