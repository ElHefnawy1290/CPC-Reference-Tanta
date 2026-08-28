/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: 3D MO'S ALGORITHM (WITH UPDATES)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Point Updates", "Range Queries", "Offline", "Distinct elements with updates".
 * - Classic Scenarios: You are given an array of size N. You have two types of queries:
 *   Type 1: Query the number of distinct elements in range [L, R].
 *   Type 2: Change the element at index X to V.
 * - The Magic: A standard Segment Tree cannot count "Distinct Elements". Standard Mo's
 *   Algorithm cannot handle "Updates". 3D Mo combines them by adding a "Time" pointer (T).
 *   Queries are sorted into 3D blocks: [L_block, R_block, Time].
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization:
 *   As you read the input, split it! If it's an update, push to the `updates` array.
 *   If it's a query, push to the `queries` array and record the CURRENT size of the
 *   `updates` array as its "Time" stamp!
 *
 * - Complexity:
 *       Time: O(N^{5/3}) optimally.
 *       Space: O(N + Q) memory.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - The Block Size (B):
 *   Standard Mo uses B = N^{1/2}. 3D Mo MUST use B = N^{2/3}.
 *   If N = 100,000, N^{2/3} is roughly 2154. The template calculates this dynamically!
 * - Coordinate Compression:
 *   This template assumes array values are <= 10^6. If values are up to 10^9, you MUST
 *   coordinate compress the initial array AND all the update values before running Mo!
 */

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;
const int MAX_VAL = 1000005; // Maximum possible value in the array

int n, q;
int a[MAXN];         // The active array
int freq[MAX_VAL];   // Frequency array for distinct element counting
int current_ans = 0; // Tracks the current number of distinct elements
int BLOCK_SIZE;

struct Update
{
    int idx;
    int val; // When an update is applied, this holds the NEW value.
             // When reverted, it automatically swaps to hold the OLD value!
};

struct Query
{
    int l, r, t, id;
    int l_block, r_block;

    // Sort by L block, then R block, then Time.
    // Includes a 3D parity optimization for faster pointer movement!
    bool operator<(const Query &other) const
    {
        if (l_block != other.l_block)
            return l_block < other.l_block;

        if (r_block != other.r_block)
        {
            return (l_block & 1) ? r_block < other.r_block : r_block > other.r_block;
        }

        return (r_block & 1) ? t < other.t : t > other.t;
    }
};

vector<Update> updates;
vector<Query> queries;
vector<int> ans;

// Add an element at index 'idx' to our active window
inline void add(int idx)
{
    if (freq[a[idx]] == 0)
    {
        current_ans++;
    }
    freq[a[idx]]++;
}

// Remove an element at index 'idx' from our active window
inline void remove(int idx)
{
    freq[a[idx]]--;
    if (freq[a[idx]] == 0)
    {
        current_ans--;
    }
}

// ? THE MAGIC: The Swap Trick
// If we move forward in time, we swap the current array value with the update's value.
// Because we swap, the `updates[t_idx].val` now holds the OLD value!
// This means if we need to travel backward in time, we just call the exact same function!
inline void apply_time(int t_idx, int L, int R)
{
    int idx = updates[t_idx].idx;

    // If the index being updated is currently INSIDE our active [L, R] window,
    // we must remove its contribution using the old value.
    if (L <= idx && idx <= R)
    {
        remove(idx);
    }

    // Swap the array value and the update cache!
    swap(a[idx], updates[t_idx].val);

    // If the index is in our active window, add its contribution using the new value!
    if (L <= idx && idx <= R)
    {
        add(idx);
    }
}

void solve()
{
    if (!(cin >> n >> q))
        return;

    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
    }

    // 1-based indexing for updates is safer for the Time pointer
    updates.push_back({0, 0}); // Dummy update at T = 0

    // Parse queries and updates
    for (int i = 0; i < q; i++)
    {
        int type;
        cin >> type;
        if (type == 1)
        {
            // Type 1: Range Query (L, R)
            int l, r;
            cin >> l >> r;
            queries.push_back({l, r, (int)updates.size() - 1, (int)queries.size(), 0, 0});
        }
        else
        {
            // Type 2: Point Update (Index X becomes Value V)
            int idx, val;
            cin >> idx >> val;
            updates.push_back({idx, val});
        }
    }

    // ? THE MATH: Optimal block size for 3D Mo is N^(2/3)
    BLOCK_SIZE = max(1, (int)pow(n, 2.0 / 3.0));

    for (auto &qry : queries)
    {
        qry.l_block = qry.l / BLOCK_SIZE;
        qry.r_block = qry.r / BLOCK_SIZE;
    }

    sort(queries.begin(), queries.end());
    ans.resize(queries.size());

    int L = 1, R = 0, T = 0;

    for (const auto &qry : queries)
    {
        // Step 1: Expand the spatial window [L, R]
        while (L > qry.l)
        {
            L--;
            add(L);
        }
        while (R < qry.r)
        {
            R++;
            add(R);
        }

        // Step 2: Shrink the spatial window [L, R]
        while (L < qry.l)
        {
            remove(L);
            L++;
        }
        while (R > qry.r)
        {
            remove(R);
            R--;
        }

        // Step 3: Fast-forward or Rewind the Time window (T)
        // We do Time AFTER Space to ensure the active window bounds are correct
        // for the `apply_time` intersection check!
        while (T < qry.t)
        {
            T++;
            apply_time(T, L, R);
        }
        while (T > qry.t)
        {
            apply_time(T, L, R);
            T--;
        }

        // Record the answer for this specific snapshot in space and time
        ans[qry.id] = current_ans;
    }

    for (int i = 0; i < ans.size(); i++)
    {
        cout << ans[i] << "\n";
    }
}

int main()
{
    // Fast I/O is mandatory due to heavy pointer manipulation
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}