/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: TREE REROOTING DP (IN-OUT DP)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Sum of distances to all nodes", "Tree Rerooting", "Tree DP", "Calculate for EVERY node".
 * - Classic Scenarios: You are given a tree and asked to find the sum of distances from node U to 
 *   ALL other nodes, but you need to output this for EVERY node U in the tree. A naive BFS/DFS 
 *   from every single node takes O(N^2), which gives TLE.
 * - The Magic: "Rerooting Technique". We solve it in two linear passes:
 *   1. Bottom-Up (dfs): Root the tree arbitrarily at node 1. Calculate the subtree size (`sz`) 
 *      and the sum of distances from node 1 to all nodes in its subtree (`dist`).
 *   2. Top-Down (dfs2): Shift the root from parent `u` to child `v` in O(1) time! 
 *      When we move the root from `u` to `v`, all nodes in `v`'s subtree become 1 step CLOSER 
 *      (so we subtract `sz[v]`), and all other nodes in the tree become 1 step FURTHER 
 *      (so we add `n - sz[v]`).
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: 
 *       Ensure `GRAPH` is defined (e.g., `typedef vector<vector<int>> GRAPH;`).
 *       The `solve()` function automatically initializes all vectors to size `N + 1`.
 *
 * - Execution: 
 *       1. Build the undirected graph.
 *       2. Call `dfs(1, 0)` to precalculate subtree sizes and the answer for node 1.
 *       3. Set the base case: `ans[1] = dist[1]`.
 *       4. Call `dfs2(1, 0)` to magically propagate the answer to all other nodes.
 *
 * - Complexity:
 *       Time: O(N) — Just two simple DFS traversals.
 *       Space: O(N) for the tree, sizes, and distances.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Weighted Edges?
 *   If edges have weights `W`, change `graph` to store pairs `{neighbor, weight}`.
 *   In `dfs1`: `dist[u] += dist[v] + sz[v] * W`.
 *   In `dfs2`: `ans[v] = ans[u] - sz[v] * W + (n - sz[v]) * W`.
 * - Max Distance instead of Sum?
 *   Rerooting can also find the farthest node from every node! However, the transition is harder: 
 *   you must keep track of the MAXIMUM and SECOND MAXIMUM depths in `dfs1` so that when you reroot 
 *   into the branch that contained the maximum, you can fall back to the second maximum.
 */
int n;
vector<int> sz, dist, ans;
GRAPH graph;

void dfs(int u, int p)
{
    sz[u] = 1;
    for (auto &v : graph[u])
    {
        if (v == p)
            continue;
        dfs(v, u);
        sz[u] += sz[v];
        dist[u] += dist[v] + sz[v];
    }
}

void dfs2(int u, int p)
{
    for (auto &v : graph[u])
    {
        if (v == p)
            continue;
        ans[v] = ans[u] - sz[v] + (n - sz[v]);
        dfs2(v, u);
    }
}

void solve()
{
    cin >> n;
    sz.assign(n + 1, {}), dist.assign(n + 1, {}), graph.assign(n + 1, {}), ans.assign(n + 1, {});
    for (int i = 1; i < n; i++)
    {
        int a, b;
        cin >> a >> b;
        graph[a].emplace_back(b), graph[b].emplace_back(a);
    }
    dfs(1, 0);
    ans[1] = dist[1];
    dfs2(1, 0);
    for (int i = 1; i <= n; i++)
        cout << ans[i] << " ";
}
