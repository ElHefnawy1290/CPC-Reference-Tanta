/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: BRIDGES & ARTICULATION POINTS (CUT VERTICES & EDGES)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Critical connections", "Single point of failure", "Vulnerable roads/servers", "Network disconnection".
 * - Classic Scenarios: You have a network of cities and bidirectional roads. You need to find which 
 *   specific road (Bridge) or which specific city (Articulation Point), if destroyed, would split the 
 *   network into two or more disconnected components. Naively removing each edge/node and running DFS 
 *   takes O(E * (V + E)), which yields TLE for large graphs.
 * - The Magic: "Discovery Time (disc) and Lowest Reachable Time (low)". As we do a DFS, we timestamp 
 *   when we visit each node (`disc`). We also track the "oldest" ancestor we can reach via a back-edge (`low`).
 *   - BRIDGE: If a child `v` has `low[v] > disc[u]`, it means the subtree at `v` has NO back-edges reaching 
 *     above `u` or even to `u`. The edge `u-v` is its ONLY lifeline!
 *   - ARTICULATION POINT (AP): If `low[v] >= disc[u]`, the subtree at `v` can at best reach `u`, but nothing 
 *     above it. Destroying `u` traps `v`'s subtree!
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Clear the `adj`, `bridges`, `articulation_points`, `disc`, and `low` arrays. 
 *   Set `timer = 0`.
 * - Build Graph: Add undirected edges to the `adj` list.
 * - Execution: Call `dfs(i, -1)` for all unvisited nodes (to handle disconnected graphs).
 *       for(int i = 1; i <= n; i++) {
 *           if(!disc[i]) dfs(i, -1);
 *       }
 *
 * - Complexity:
 *       Time: O(V + E) — Just a single DFS traversal!
 *       Space: O(V + E) for adjacency list, recursion stack, and tracking arrays.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - ⚠️ MULTIPLE EDGES (MULTIGRAPH) TRAP: If there are multiple direct edges between `u` and `v`, 
 *   the condition `if(v == p)` is dangerous because it blocks ALL edges back to the parent, even valid 
 *   parallel edges. FIX: Pass the `edge_index` instead of the parent `node` to the DFS, and skip only 
 *   the specific edge that brought you to `u`.
 * - Why a `set` for Articulation Points? A node might be an AP for multiple different branches of its 
 *   subtree. Pushing to a vector would cause duplicates. `std::set` handles this automatically, but if 
 *   you need more speed, use a boolean array `is_AP[MAXN]` instead.
 * - Bridge Tree (2-Edge-Connected Components): After finding all bridges, if you remove them from the graph, 
 *   the remaining connected components can be "shrunk" into single super-nodes. The resulting graph 
 *   will always be a perfect Tree!
 */
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5 + 5;
vector<int> adj[MAXN];
int disc[MAXN], low[MAXN], timer;
vector<pair<int, int>> bridges;
set<int> articulation_points;

void dfs(int u, int p = -1)
{
    disc[u] = low[u] = ++timer;
    int children = 0; // Needed for the root case

    for (int v : adj[u])
    {
        if (v == p)
            continue; // Don't go back to parent

        if (disc[v])
        {
            // Back-edge found!
            low[u] = min(low[u], disc[v]);
        }
        else
        {
            // Tree-edge
            dfs(v, u);
            low[u] = min(low[u], low[v]);

            // BRIDGE CONDITION
            if (low[v] > disc[u])
                bridges.push_back({u, v});

            // ARTICULATION POINT CONDITION
            if (low[v] >= disc[u] && p != -1)
                articulation_points.insert(u);

            children++;
        }
    }

    // ROOT SPECIAL CASE: If root has > 1 child, it's an AP
    if (p == -1 && children > 1)
        articulation_points.insert(u);
}
