/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: BABY-STEP GIANT-STEP (DISCRETE LOG)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Discrete Logarithm", "Find the exponent", "A^x = B mod M".
 * - Classic Scenarios: You are given A, B, and M, and need to find the minimum
 *   non-negative integer x such that A^x ≡ B (mod M).
 * - The Magic: It trades space for time. By rewriting x = n*p - q (where n = sqrt(M)),
 *   the equation becomes A^(n*p) ≡ B * A^q (mod M). We precompute the right side
 *   (Baby Steps) and store them in a hash map, then calculate the left side
 *   (Giant Steps) and check for collisions.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       long long ans = solve_bsgs(A, B, M);
 *       // Returns -1 if no solution exists.
 *
 * - Complexity:
 *       Time: O(sqrt(M) * log(sqrt(M))) due to the map. O(sqrt(M)) if using unordered_map.
 *       Space: O(sqrt(M)) to store the baby steps.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Coprime Requirement: This standard version assumes gcd(A, M) == 1. If A and M
 *   are not coprime, you need the Extended BSGS algorithm (which involves dividing
 *   out the GCDs until they are coprime).
 */

using ll = long long;

ll solve_bsgs(ll a, ll b, ll m)
{
    a %= m, b %= m;
    if (b == 1 || m == 1)
        return 0;

    ll n = sqrt(m) + 1;
    map<ll, ll> vals;
    ll cur = b;
    for (ll q = 0; q <= n; ++q)
    {
        vals[cur] = q;
        cur = (cur * a) % m;
    }

    ll a_n = 1;
    for (ll i = 0; i < n; ++i)
    {
        a_n = (a_n * a) % m;
    }

    cur = 1;
    for (ll p = 1; p <= n; ++p)
    {
        cur = (cur * a_n) % m;
        if (vals.count(cur))
        {
            ll ans = n * p - vals[cur];
            return ans;
        }
    }
    return -1;
}