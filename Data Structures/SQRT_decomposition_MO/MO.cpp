#include <bits/stdc++.h>
using namespace std;

// Global variables for Mo's Algorithm
int BLOCK_SIZE;
int current_answer = 0; // Or 1 if doing multiplication

struct Query
{
    int l, r, id;

    // The Even/Odd Sorting Trick
    // This makes the R pointer snake up and down, saving 50% of execution time!
    bool operator<(const Query &other) const
    {
        int block_own = l / BLOCK_SIZE;
        int block_other = other.l / BLOCK_SIZE;
        if (block_own != block_other)
            return block_own < block_other;
        return (block_own & 1) ? (r < other.r) : (r > other.r);
    }
};

void add(int idx)
{
    // YOUR LOGIC HERE: Element added to the window
    // e.g., if (++cnt[a[idx]] == 1) current_answer++;
}

void remove(int idx)
{
    // YOUR LOGIC HERE: Element removed from the window
    // e.g., if (--cnt[a[idx]] == 0) current_answer--;
}

void solve()
{
    int n, q;
    cin >> n >> q;

    // Read your array elements here
    vector<int> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];

    // 1. Calculate optimal block size
    BLOCK_SIZE = max(1, (int)(n / sqrt(q)));

    vector<Query> queries(q);
    for (int i = 0; i < q; i++)
    {
        cin >> queries[i].l >> queries[i].r;

        // Convert to 0-based indexing if the problem uses 1-based!
        queries[i].l--;
        queries[i].r--;

        queries[i].id = i;
    }

    // 2. Sort the queries
    sort(queries.begin(), queries.end());

    vector<int> ans(q);

    // 3. Initialize pointers to an empty window
    int L = 0, R = -1;

    // 4. Answer queries
    for (Query q : queries)
    {
        // ALWAYS EXPAND FIRST (add elements)
        while (L > q.l)
        {
            L--;
            add(L);
        }
        while (R < q.r)
        {
            R++;
            add(R);
        }

        // THEN SHRINK (remove elements)
        while (L < q.l)
        {
            remove(L);
            L++;
        }
        while (R > q.r)
        {
            remove(R);
            R--;
        }

        ans[q.id] = current_answer;
    }

    // Print answers in original order
    for (int i = 0; i < q; i++)
    {
        cout << ans[i] << "\n";
    }
}
