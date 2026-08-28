/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: LOWER BOUND ON BIT (BINARY LIFTING)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Find the K-th smallest element", "First index with prefix sum >= X".
 * - Classic Scenarios: You have an array of frequencies (how many times a number appears).
 *   You need to dynamically add/remove numbers and frequently ask: "What is the K-th
 *   smallest number currently in my set?"
 * - The Magic: Normally, you'd use a C++ PBDS (Policy Based Data Structure) for this.
 *   If PBDS is banned, or you need raw execution speed, this algorithm does a binary
 *   search directly ON the Fenwick tree's internal structure. It takes exactly O(log N)
 *   steps instead of O(log^2 N).
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Pass the size of the array to the constructor.
 *       FenwickTreeLB fenwick(N);
 *
 * - Standard Point Updates (1-indexed):
 *       // Add 'val' to index 'idx'
 *       fenwick.update(idx, val);
 *
 * - The K-th Element Query (Lower Bound):
 *       // Find the smallest index where the prefix sum is >= V
 *       int ans_idx = fenwick.lower_bound(V);
 *
 * - Complexity:
 *       Time: O(log N) per update and lower_bound query.
 *       Space: O(N) memory.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - ⚠️ CRITICAL RULE: MONOTONICITY
 *   This function ONLY works if the original array contains ONLY non-negative numbers!
 *   If you add negative numbers, the prefix sums go up and down randomly. Binary search
 *   (and therefore this binary lifting trick) will completely fail.
 * - Out of Bounds Check:
 *   If you search for a value `V` that is larger than the total sum of the entire array,
 *   the function will return `N + 1`. Always check `if (ans_idx > N)` if you aren't sure
 *   the sum exists!
 */

#include <bits/stdc++.h>
using namespace std;

struct FenwickTreeLB
{
private:
    int n;
    int LOGN;
    vector<long long> bit;

public:
    FenwickTreeLB(int size)
    {
        n = size;
        bit.assign(n + 1, 0);

        // Find the highest power of 2 that is <= n
        LOGN = 0;
        while ((1 << (LOGN + 1)) <= n)
        {
            LOGN++;
        }
    }

    // Add `val` to the cell at `idx`
    void update(int idx, long long val)
    {
        for (; idx <= n; idx += idx & -idx)
        {
            bit[idx] += val;
        }
    }

    // Get standard prefix sum up to `idx`
    long long query(int idx)
    {
        long long sum = 0;
        for (; idx > 0; idx -= idx & -idx)
        {
            sum += bit[idx];
        }
        return sum;
    }

    // ? THE MAGIC: Binary Lifting directly on the Fenwick Tree
    // Returns the 1-based index of the FIRST element whose prefix sum is >= v
    int lower_bound(long long v)
    {
        if (v <= 0)
            return 1; // Edge case: Target is already reached at the start

        int idx = 0;
        long long current_sum = 0;

        // Jump down from the highest power of 2 to 1
        for (int i = LOGN; i >= 0; i--)
        {
            int next_idx = idx + (1 << i);

            // If taking this jump keeps us inside the array bounds,
            // AND the sum AT this jump is still STRICTLY LESS than our target `v`
            if (next_idx <= n && current_sum + bit[next_idx] < v)
            {
                // We commit to the jump!
                idx = next_idx;
                current_sum += bit[next_idx];
            }
        }

        // `idx` is now the largest index where the sum is STRICTLY LESS than `v`.
        // Therefore, `idx + 1` is the first index where the sum is >= `v`!
        return idx + 1;
    }
};

void solve()
{
    int n, q;
    // Example Input Format: Array of size N, Q queries
    if (!(cin >> n >> q))
        return;

    FenwickTreeLB fenwick(n);

    while (q--)
    {
        int type;
        cin >> type;

        if (type == 1)
        { // Point Add
            int idx;
            long long val;
            cin >> idx >> val;
            fenwick.update(idx, val);
        }
        else if (type == 2)
        { // Lower Bound Query
            long long target_sum;
            cin >> target_sum;

            int ans = fenwick.lower_bound(target_sum);

            if (ans > n)
            {
                cout << "Target sum not reached!\n";
            }
            else
            {
                cout << "Index: " << ans << "\n";
            }
        }
    }
}

int main()
{
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}