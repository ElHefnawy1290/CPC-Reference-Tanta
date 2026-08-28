/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: ROLLBACK MO'S ALGORITHM
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Range queries", "Add is easy, Remove is impossible", "DSU on ranges".
 * - Classic Scenarios: You are given a graph with N edges. You receive queries asking:
 *   "If we only use edges from index L to R, how many connected components are there?"
 * - The Magic: Standard Mo's algorithm requires you to slide both the L and R pointers
 *   by adding AND removing elements. But you cannot "remove" an edge from a standard DSU!
 *   Rollback Mo sorts the queries so that the R pointer ONLY moves forward (add only).
 *   The L pointer moves backward temporarily, and we "undo" those L moves using a stack
 *   history (Rollback) before moving to the next query. NO `remove()` function needed!
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Define your state: You need a data structure that supports `add()`, `save_state()`,
 *   and `rollback(state)`. (I have included a Rollback DSU in this template).
 * - Read Queries: Push all queries into a vector and pass them to the Mo's runner.
 *
 * - Complexity:
 *       Time: O(Q * sqrt(N) * Cost(Add)). If DSU, it's O(Q * sqrt(N) * log N).
 *       Space: O(N + Q) memory.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - The Block Size (B):
 *   The math for Mo's dictates the block size should be roughly N / sqrt(Q).
 *   If the problem has N = 10^5, setting `B = 350` or `B = 400` is the standard sweet spot.
 * - Changing the Data Structure:
 *   If the problem asks for "Max frequency in range", you don't need DSU. You just need
 *   an array tracking frequencies, and a variable `max_freq`. To rollback, you just
 *   remember the old `max_freq` and revert the array.
 */

#include <bits/stdc++.h>
using namespace std;

// 1. The Rollback Data Structure
struct RollbackDSU
{
    vector<int> parent, sz;
    int components;

    // We store the history of changes: {node_u, node_v, previous_components}
    struct Change
    {
        int u, v, comps;
    };
    stack<Change> history;

    RollbackDSU(int n)
    {
        parent.resize(n + 1);
        sz.assign(n + 1, 1);
        iota(parent.begin(), parent.end(), 0);
        components = n;
    }

    // ! CRITICAL: No Path Compression allowed! Only Union by Size.
    int find(int i)
    {
        while (i != parent[i])
            i = parent[i];
        return i;
    }

    void unite(int u, int v)
    {
        int root_u = find(u);
        int root_v = find(v);

        if (root_u == root_v)
        {
            // Even if they are already connected, we push a dummy change to keep
            // the stack height aligned with the number of `add()` calls.
            history.push({-1, -1, components});
            return;
        }

        if (sz[root_u] < sz[root_v])
            swap(root_u, root_v);

        // Record the exact state BEFORE we change it
        history.push({root_u, root_v, components});

        // Apply changes
        parent[root_v] = root_u;
        sz[root_u] += sz[root_v];
        components--;
    }

    // Returns the current "timestamp" or "height" of the history stack
    int save()
    {
        return history.size();
    }

    // Undos all operations until the stack shrinks back to `target_time`
    void rollback(int target_time)
    {
        while (history.size() > target_time)
        {
            auto [u, v, old_comps] = history.top();
            history.pop();

            if (u != -1)
            {
                sz[u] -= sz[v];
                parent[v] = v;
            }
            components = old_comps;
        }
    }

    void reset(int n)
    {
        while (!history.empty())
            history.pop();
        for (int i = 1; i <= n; i++)
        {
            parent[i] = i;
            sz[i] = 1;
        }
        components = n;
    }
};

// 2. Query Structure and Mo's Sorting logic
int BLOCK_SIZE;

struct Query
{
    int l, r, id;

    // Sort queries by their Left Block, then by their Right Endpoint
    bool operator<(const Query &other) const
    {
        int block_own = l / BLOCK_SIZE;
        int block_other = other.l / BLOCK_SIZE;
        if (block_own != block_other)
        {
            return block_own < block_other;
        }
        // If in the same block, sort R strictly ascending
        return r < other.r;
    }
};

struct Edge
{
    int u, v;
};

void solve()
{
    int n, m, q;
    // N vertices, M edges, Q queries
    if (!(cin >> n >> m >> q))
        return;

    // Standard block size for 10^5 is roughly 350
    BLOCK_SIZE = max(1, (int)sqrt(m));

    vector<Edge> edges(m + 1); // 1-indexed edges
    for (int i = 1; i <= m; i++)
    {
        cin >> edges[i].u >> edges[i].v;
    }

    vector<Query> queries(q);
    for (int i = 0; i < q; i++)
    {
        cin >> queries[i].l >> queries[i].r;
        queries[i].id = i;
    }

    // 1. Sort the queries
    sort(queries.begin(), queries.end());

    vector<int> ans(q);
    RollbackDSU dsu(n);

    int last_block = -1;
    int current_r = 0;

    for (const Query &query : queries)
    {
        int current_block = query.l / BLOCK_SIZE;

        // If we entered a brand new block, completely wipe the DSU clean!
        if (current_block != last_block)
        {
            dsu.reset(n);
            // Set our R pointer to the very edge of the current block
            current_r = min(m, (current_block + 1) * BLOCK_SIZE);
            last_block = current_block;
        }

        // SCENARIO 1: The query is incredibly tiny and fits entirely inside one block
        // We answer it naively in O(Sqrt(N)) and instantly roll it back.
        if (query.r < (current_block + 1) * BLOCK_SIZE)
        {
            int time_stamp = dsu.save();
            for (int i = query.l; i <= query.r; i++)
            {
                dsu.unite(edges[i].u, edges[i].v);
            }
            ans[query.id] = dsu.components;
            dsu.rollback(time_stamp);
            continue;
        }

        // SCENARIO 2: The query stretches outside the block.
        // Step A: Expand our R pointer forward until it hits the query's R limit.
        // Notice we NEVER rollback these operations while staying in the same block!
        while (current_r <= query.r)
        {
            dsu.unite(edges[current_r].u, edges[current_r].v);
            current_r++;
        }

        // Step B: We temporarily drag the L pointer backward from the block edge
        // to cover the left side of the query.
        int time_stamp = dsu.save();
        for (int i = query.l; i < (current_block + 1) * BLOCK_SIZE; i++)
        {
            dsu.unite(edges[i].u, edges[i].v);
        }

        // Step C: Record the answer!
        ans[query.id] = dsu.components;

        // Step D: Rollback the temporary L pointer movements.
        // The R pointer progress remains saved for the next queries!
        dsu.rollback(time_stamp);
    }

    for (int i = 0; i < q; i++)
    {
        cout << ans[i] << "\n";
    }
}

int main()
{
    // Fast I/O is mandatory for heavy Mo's algorithm problems
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}