/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: PARTIALLY PERSISTENT DSU
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "At what time did U and V connect?", "Were they connected after K edges?", "Historical connectivity".
 * - Classic Scenarios: You have a graph where edges are slowly being built over time.
 *   You get queries asking if two cities were connected at a specific year in the past.
 * - The Magic: Because we ONLY use Union-by-Size (and strictly ban path compression), the DSU
 *   naturally forms a tree with a maximum depth of O(log N). By simply recording the `time`
 *   an edge was formed on the child node pointing to the parent, we can "time travel".
 *   To find the root at time T, we just climb the parent pointers, but we STOP climbing if
 *   the edge above us was built AFTER time T!
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Pass the number of nodes (1-indexed).
 *       PartiallyPersistentDSU pp_dsu(N);
 *
 * - Updates: Merge U and V at time T. (T must be strictly increasing across calls).
 *       pp_dsu.unite(U, V, T);
 *
 * - Queries:
 *       // Were U and V connected at time T?
 *       bool connected = pp_dsu.isConnected(U, V, T);
 *
 *       // What is the EXACT time U and V became connected?
 *       int time_joined = pp_dsu.timeConnected(U, V);
 *
 * - Complexity:
 *       Time: O(log N) per merge and per query.
 *       Space: O(N) memory.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - 0-indexed nodes?
 *   The template naturally allocates `n + 1` space. It works flawlessly for both 0-indexed
 *   and 1-indexed inputs without changing any code!
 * - Getting the size of a component at time T?
 *   This requires a bit more work. You would need to make `sz` a vector of `vector<pair<int, int>>`
 *   to store the history of sizes: `sz[root].push_back({time, new_size})`, and then binary search it.
 *   If you just need standard connectivity, this template is perfectly lightweight.
 */

#include <bits/stdc++.h>
using namespace std;

struct PartiallyPersistentDSU
{
private:
    vector<int> parent;
    vector<int> sz;
    vector<int> time_joined; // The time this node became a child of its parent
    const int INF = 2e9 + 7;

public:
    PartiallyPersistentDSU(int n)
    {
        parent.resize(n + 1);
        sz.assign(n + 1, 1);
        time_joined.assign(n + 1, INF);
        iota(parent.begin(), parent.end(), 0);
    }

    // ? CRITICAL: Notice there is NO path compression `parent[i] = find(...)`!
    // Path compression would destroy the historical edges.
    int find(int i, int t)
    {
        // We stop climbing if we hit a root, or if the edge above us hasn't been built yet at time `t`
        if (i == parent[i] || time_joined[i] > t)
        {
            return i;
        }
        return find(parent[i], t);
    }

    // Merge u and v at time t. (Assumes t is strictly increasing as you feed edges)
    bool unite(int u, int v, int t)
    {
        // When merging, we only care about the present moment, so we pass INF as time
        int root_u = find(u, INF);
        int root_v = find(v, INF);

        if (root_u == root_v)
            return false;

        // Union by Size ensures the tree depth stays O(log N)
        if (sz[root_u] < sz[root_v])
        {
            swap(root_u, root_v);
        }

        parent[root_v] = root_u;
        sz[root_u] += sz[root_v];

        // ? THE MAGIC: We record EXACTLY when root_v surrendered to root_u
        time_joined[root_v] = t;

        return true;
    }

    // Check if u and v were in the same component at time t
    bool isConnected(int u, int v, int t)
    {
        return find(u, t) == find(v, t);
    }

    // Find the exact time u and v became connected. Returns INF if they never connect.
    int timeConnected(int u, int v)
    {
        if (!isConnected(u, v, INF))
            return INF; // Never connected

        // Binary search the timeline to find the exact moment of connection!
        int l = 0, r = INF - 1;
        int ans = INF;

        while (l <= r)
        {
            int mid = l + (r - l) / 2;
            if (isConnected(u, v, mid))
            {
                ans = mid;
                r = mid - 1; // Try to find an earlier time
            }
            else
            {
                l = mid + 1; // We need more time
            }
        }
        return ans;
    }
};

void solve()
{
    int n, m, q;
    // Example Input: N nodes, M edges added sequentially, Q queries
    if (!(cin >> n >> m >> q))
        return;

    PartiallyPersistentDSU pp_dsu(n);

    for (int i = 1; i <= m; i++)
    {
        int u, v;
        cin >> u >> v;
        // Assume edges are added at time `i`
        pp_dsu.unite(u, v, i);
    }

    while (q--)
    {
        int type, u, v;
        cin >> type >> u >> v;

        if (type == 1)
        {
            // Were u and v connected at time T?
            int t;
            cin >> t;
            cout << (pp_dsu.isConnected(u, v, t) ? "YES" : "NO") << "\n";
        }
        else
        {
            // At what time did u and v finally connect?
            int t = pp_dsu.timeConnected(u, v);
            if (t > m)
                cout << "Not connected\n";
            else
                cout << "Connected at edge " << t << "\n";
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    // solve();
    return 0;
}