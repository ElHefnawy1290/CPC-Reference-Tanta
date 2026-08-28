/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: CUBIC ROOT DIVISOR SUM
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Sum of Divisors", "Sigma0(i)", "Sum of floor(N/i)".
 * - Classic Scenarios: You are asked to compute sum_{i=1}^{N} d(i), which is
 *   mathematically equivalent to sum_{i=1}^{N} floor(N / i), for a staggering N = 10^18.
 * - The Magic: The naive O(N) loop crashes. The standard Dirichlet hyperbola
 *   optimization takes O(sqrt(N)), which runs in ~1 second for 10^12 but fails at 10^18.
 *   This extremely advanced algorithm uses Stern-Brocot tree fraction approximation
 *   to step through the hyperbola in blocks, reducing the time to roughly O(N^(1/3))!
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       __uint128_t ans = sum_sigma0(N);
 *       // Remember to write a custom print function for __uint128_t if outputting directly.
 *
 * - Complexity:
 *       Time: ~O(N^(1/3)). Easily executes N = 10^18 in < 100 milliseconds.
 *       Space: O(log N) for the stack depth.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - 128-bit Types: Be very careful if modifying the inner loop logic. The condition
 *   `uint128(x) * x * dy >= uint128(n) * dx` uses 128-bit integers
 *   because cubing boundaries near 10^18 guarantees overflow in standard `uint64_t`.
 */

using uint32 = unsigned int;
using uint64 = unsigned long long;
using uint128 = __uint128_t;

uint128 sum_sigma0(uint64 n)
{
    auto out = [n](uint64 x, uint32 y)
    { return x * y > n; };
    auto cut = [n](uint64 x, uint32 dx, uint32 dy)
    { return uint128(x) * x * dy >= uint128(n) * dx; };
    const uint64 sn = sqrtl(n);
    const uint64 cn = pow(n, 0.34);
    uint64 x = n / sn;
    uint32 y = n / x + 1;
    uint128 ret = 0;
    stack<pair<uint32, uint32>> st;
    st.emplace(1, 0);
    st.emplace(1, 1);
    while (true)
    {
        uint32 lx, ly;
        tie(lx, ly) = st.top();
        st.pop();
        while (out(x + lx, y - ly))
        {
            ret += x * ly + uint64(ly + 1) * (lx - 1) / 2;
            x += lx, y -= ly;
        }
        if (y <= cn)
            break;
        uint32 rx = lx, ry = ly;
        while (true)
        {
            tie(lx, ly) = st.top();
            if (out(x + lx, y - ly))
                break;
            rx = lx, ry = ly;
            st.pop();
        }
        while (true)
        {
            uint32 mx = lx + rx, my = ly + ry;
            if (out(x + mx, y - my))
            {
                st.emplace(lx = mx, ly = my);
            }
            else
            {
                if (cut(x + mx, lx, ly))
                    break;
                rx = mx, ry = my;
            }
        }
    }
    for (--y; y > 0; --y)
        ret += n / y;
    return ret * 2 - sn * sn;
}