/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: INCLUSION-EXCLUSION (PIE) ON MULTIPLES
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Divisible by any", "Coprime to array", "Multiples below X".
 * - Classic Scenarios: You are given a target number X, and an array of K integers.
 *   You need to find how many numbers from 1 to X are divisible by AT LEAST ONE
 *   of the integers in the array.
 * - The Magic: It systematically adds numbers divisible by 1 element, subtracts
 *   those divisible by 2 elements, adds for 3, etc. A naive bitmask loop goes out
 *   of bounds (LCM overflow) quickly. This DFS safely prunes branches if the LCM
 *   ever exceeds X, making it insanely fast and overflow-proof.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       long long ans = count_divisible(X, A);
 *       // To find numbers COPRIME to the array: ans = X - count_divisible(X, A);
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Array Cleaning: For maximum speed, it is highly recommended to sort the array `A`,
 *   remove duplicates, and remove any number that is a multiple of another number
 *   in the array before calling this function.
 */

using ll = long long;

ll pie_dfs(int idx, ll current_lcm, int elements_chosen, ll max_val, const vector<ll> &a)
{
    if (idx == a.size())
    {
        if (elements_chosen == 0)
            return 0;
        if (elements_chosen % 2 == 1)
            return max_val / current_lcm;
        return -(max_val / current_lcm);
    }

    ll ans = pie_dfs(idx + 1, current_lcm, elements_chosen, max_val, a);

    ll g = __gcd(current_lcm, a[idx]);
    if (current_lcm / g <= max_val / a[idx])
    {
        ans += pie_dfs(idx + 1, (current_lcm / g) * a[idx], elements_chosen + 1, max_val, a);
    }

    return ans;
}

ll count_divisible(ll max_val, const vector<ll> &a)
{
    return pie_dfs(0, 1, 0, max_val, a);
}