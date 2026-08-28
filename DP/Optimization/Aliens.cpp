#include <bits/stdc++.h>

using namespace std;

#define El_Hefnawys                   \
    ios_base::sync_with_stdio(false); \
    cin.tie(nullptr);                 \
    cout.tie(nullptr)

typedef long long ll;
#define int ll

const int OO = 1e15;

// choose k subarrays from the array and trying to maximize the sum
// dp[i][0/1]

// The DP function returns a pair:
// 1. The maximum sum (including the penalties applied)
// 2. The NEGATIVE number of subarrays used (to break ties by preferring fewer subarrays)
pair<int, int> check(int penalty, int n, const vector<int> &a)
{
    // dp[i][0] = max sum up to index i, currently NOT in a subarray
    // dp[i][1] = max sum up to index i, currently INSIDE a subarray
    vector<vector<pair<int, int>>> dp(n, vector<pair<int, int>>(2));

    dp[0][0] = {0, 0};
    dp[0][1] = {a[0] - penalty, -1}; // Starting an array costs 'penalty'

    for (int i = 1; i < n; i++)
    {
        // Option 0: We are not in a subarray.
        // We either were already not in one, or we just closed the previous one.
        dp[i][0] = max(dp[i - 1][0], dp[i - 1][1]);

        // Option 1: We are inside a subarray.
        // Transition A: Start a brand NEW subarray today (costs penalty, adds -1 to count)
        pair<int, int> start_new = make_pair(
            dp[i - 1][0].first - penalty + a[i],
            dp[i - 1][0].second - 1);

        // Transition B: Extend the EXISTING subarray (no penalty, count doesn't change)
        pair<int, int> extend_existing = make_pair(
            dp[i - 1][1].first + a[i],
            dp[i - 1][1].second);

        dp[i][1] = max(start_new, extend_existing);
    }

    return max(dp[n - 1][0], dp[n - 1][1]);
}

void solve()
{
    int n, k;
    if (!(cin >> n >> k))
        return;

    vector<int> a(n);
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }

    // The penalty bounds. Can be negative depending on the problem!
    int l = -1e14, r = 1e14;
    int best_ans = 0;

    while (l <= r)
    {
        int mid_penalty = l + (r - l) / 2;

        pair<int, int> res = check(mid_penalty, n, a);
        int arrays_used = -res.second; // Flip back to positive

        // If we used at least K arrays, this penalty is valid to consider.
        // We might have tied and used exactly K, or more.
        if (arrays_used >= k)
        {
            // Restore the true sum by adding back the penalty for EXACTLY K arrays.
            // Even if the DP used more than K due to ties (0-sum arrays),
            // the mathematical projection at the hull tangent guarantees this formula is exact!
            best_ans = res.first + (k * mid_penalty);

            // Try to force it to use FEWER arrays by INCREASING the penalty
            l = mid_penalty + 1;
        }
        else
        {
            // We used too few arrays. The penalty is too harsh. Decrease it.
            r = mid_penalty - 1;
        }
    }

    cout << best_ans << "\n";
}

signed main()
{
    El_Hefnawys;
    int t{1};
    // cin >> t;
    while (t--)
    {
        solve();
    }
    return 0;
}
