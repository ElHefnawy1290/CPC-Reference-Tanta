/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: MO'S ALGORITHM ON TREES
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Path queries", "Distinct values on path", "Offline tree queries".
 * - Classic Scenarios: You are given a tree where each node has a color. You are asked Q queries:
 *   "How many distinct colors exist on the simple path between node U and node V?"
 * - The Magic: We flatten the tree using an Euler Tour. Every node appears twice in the array:
 *   once when we enter it (tin), and once when we leave it (tout).
 *   If we want the path between U and V (where tin[U] < tin[V]), we query the flat array:
 *   A) If U is an ancestor of V: Query range [tin[U], tin[V]].
 *   B) If U is NOT an ancestor: Query range [tout[U], tin[V]] PLUS the LCA(U, V).
 *   Because dead-end branches will appear twice in this range (entry and exit), we simply
 *   TOGGLE their state. 1st appearance = Add. 2nd appearance = Remove.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Pass your tree edges and node values, then run `dfs(1, 0)`.
 * - L and R Logic: We maintain an `in_path` boolean array. The `toggle()` function handles
 *   adding OR removing automatically based on the boolean!
 *
 * - Complexity:
 *       Time: O(N log N) to setup LCA + O((N + Q) * sqrt(2N)) for Mo's processing.
 *       Space: O(N log N) for LCA lifting table + O(N) arrays.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Changing the logic:
 *   Right now, the template counts "Distinct Colors" using a frequency array.
 *   Modify the `add()` and `remove()` functions inside `toggle()` to track whatever
 *   property your specific problem requires.
 */

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;
const int LOG = 20;

int n, q;
vector<int> adj[MAXN];
long long val[MAXN]; // The color or value of each node

// Euler Tour Variables
int timer_tour = 0;
int tin[MAXN], tout[MAXN];
int tour[2 * MAXN]; // The flattened 2N array

// LCA Variables
int up[MAXN][LOG];
int depth[MAXN];

void dfs(int u, int p)
{
    // 1. Record Entry Time
    tin[u] = ++timer_tour;
    tour[timer_tour] = u;

    // 2. Build Binary Lifting Table for LCA
    up[u][0] = p;
    for (int i = 1; i < LOG; i++)
    {
        up[u][i] = up[up[u][i - 1]][i - 1];
    }

    // 3. Traverse Children
    for (int v : adj[u])
    {
        if (v != p)
        {
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
    }

    // 4. Record Exit Time
    tout[u] = ++timer_tour;
    tour[timer_tour] = u;
}

int get_lca(int u, int v)
{
    if (depth[u] < depth[v])
        swap(u, v);

    int diff = depth[u] - depth[v];
    for (int i = 0; i < LOG; i++)
    {
        if ((diff >> i) & 1)
            u = up[u][i];
    }

    if (u == v)
        return u;

    for (int i = LOG - 1; i >= 0; i--)
    {
        if (up[u][i] != up[v][i])
        {
            u = up[u][i];
            v = up[v][i];
        }
    }
    return up[u][0];
}

int BLOCK_SIZE;

struct Query
{
    int l, r, id, lca;

    bool operator<(const Query &other) const
    {
        int block_own = l / BLOCK_SIZE;
        int block_other = other.l / BLOCK_SIZE;
        if (block_own != block_other)
        {
            return block_own < block_other;
        }
        // Parity optimization for slightly faster R pointer movement
        return (block_own & 1) ? r < other.r : r > other.r;
    }
};

bool in_path[MAXN];
long long freq[MAXN]; // Example: Frequency array for colors
long long distinct_colors = 0;

// ? THE MAGIC: Toggling a node.
// If it's entering our path, we add it. If we hit its exit time, it cancels out and we remove it!
void toggle(int node)
{
    long long c = val[node];

    if (in_path[node])
    {
        // It was in the path, so we are removing it
        freq[c]--;
        if (freq[c] == 0)
            distinct_colors--;
    }
    else
    {
        // It wasn't in the path, so we are adding it
        if (freq[c] == 0)
            distinct_colors++;
        freq[c]++;
    }

    in_path[node] ^= 1; // Flip the boolean state
}

void solve()
{
    if (!(cin >> n >> q))
        return;

    // Reset globals for multiple test cases (if needed)
    timer_tour = 0;
    distinct_colors = 0;
    memset(in_path, 0, sizeof(in_path));
    memset(freq, 0, sizeof(freq));

    // Coordinate compression for colors might be required here if values > 10^5!
    for (int i = 1; i <= n; i++)
        cin >> val[i];

    for (int i = 0; i < n - 1; i++)
    {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // 1. Flatten the tree!
    depth[1] = 0;
    dfs(1, 1);

    // Block size for an array of size 2N
    BLOCK_SIZE = max(1, (int)sqrt(2 * n));

    vector<Query> queries(q);
    for (int i = 0; i < q; i++)
    {
        int u, v;
        cin >> u >> v;

        // Ensure u is visited before v in the Euler Tour
        if (tin[u] > tin[v])
            swap(u, v);

        int lca = get_lca(u, v);

        if (lca == u)
        {
            // SCENARIO A: U is an ancestor of V.
            // The path is strictly the sequence of entries from U down to V.
            queries[i].l = tin[u];
            queries[i].r = tin[v];
            queries[i].lca = -1; // No extra LCA needed
        }
        else
        {
            // SCENARIO B: U and V are in different branches.
            // We take the path from the exit of U, up to the LCA, and down to the entry of V.
            queries[i].l = tout[u];
            queries[i].r = tin[v];
            queries[i].lca = lca; // We MUST manually include the LCA!
        }
        queries[i].id = i;
    }

    // 2. Sort Queries
    sort(queries.begin(), queries.end());

    vector<long long> ans(q);

    // 3. Run Mo's Pointers on the 2N Tour Array
    int L = 1, R = 0;

    for (const Query &qry : queries)
    {
        while (L > qry.l)
        {
            L--;
            toggle(tour[L]);
        }
        while (R < qry.r)
        {
            R++;
            toggle(tour[R]);
        }
        while (L < qry.l)
        {
            toggle(tour[L]);
            L++;
        }
        while (R > qry.r)
        {
            toggle(tour[R]);
            R--;
        }

        // If U and V were on different branches, the LCA itself was NOT included
        // in the [tout[U], tin[V]] range. We must temporarily add it!
        if (qry.lca != -1)
            toggle(qry.lca);

        // Record the answer
        ans[qry.id] = distinct_colors;

        // Revert the temporary LCA addition to keep the L/R pointers clean
        if (qry.lca != -1)
            toggle(qry.lca);
    }

    for (int i = 0; i < q; i++)
    {
        cout << ans[i] << "\n";
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}