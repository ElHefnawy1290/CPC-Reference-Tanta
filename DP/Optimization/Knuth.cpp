/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: DIVIDE AND CONQUER OPTIMIZATION
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Partition array into K contiguous segments", "Minimize maximum cost".
 * - Classic Scenarios: dp[i][j] = min_{k < j} (dp[i-1][k] + cost(k, j)).
 * - The Magic: If the optimal split point `opt(i, j)` satisfies the condition
 *   opt(i, j) <= opt(i, j+1), we don't need to check all `k`. We recursively
 *   compute the middle state and restrict the search bounds for the left and
 *   right halves, dropping O(K * N^2) to O(K * N log N).
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Setup: You need two 1D arrays: `dp_prev` (results from i-1 segments)
 *   and `dp_cur` (results for i segments).
 * - Query:
 *       for (int i = 2; i <= K; i++) {
 *           compute_dc(1, N, 1, N, dp_prev, dp_cur);
 *           dp_prev = dp_cur;
 *       }
 */

using ll = long long;
const ll INF = 1e18;

// Replace this with your actual O(1) cost function (usually via prefix sums)
ll cost(int l, int r)
{
    if (l > r)
        return 0;
    return 0; // return actual cost
}

void compute_dc(int l, int r, int opt_l, int opt_r, const vector<ll> &dp_prev, vector<ll> &dp_cur)
{
    if (l > r)
        return;
    int mid = l + (r - l) / 2;
    pair<ll, int> best = {INF, -1};

    for (int k = opt_l; k <= min(mid, opt_r); k++)
    {
        ll current_cost = dp_prev[k - 1] + cost(k, mid);
        if (current_cost < best.first)
        {
            best = {current_cost, k};
        }
    }

    dp_cur[mid] = best.first;
    int opt = best.second;

    compute_dc(l, mid - 1, opt_l, opt, dp_prev, dp_cur);
    compute_dc(mid + 1, r, opt, opt_r, dp_prev, dp_cur);
}