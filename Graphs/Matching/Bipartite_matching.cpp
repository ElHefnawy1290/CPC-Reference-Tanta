#include <bits/stdc++.h>
using namespace std;

/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: HOPCROFT-KARP ALGORITHM
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Maximum Bipartite Matching", "Maximum Independent Set", "Minimum Vertex Cover".
 * - Classic Scenarios: You have two distinct groups (e.g., Workers and Jobs, or Boys and Girls).
 *   Each worker can only do specific jobs. You want to pair up as many workers with jobs as possible
 *   such that no worker gets two jobs, and no job gets two workers.
 * - The Magic: It finds the maximum matching in a bipartite graph. It works by using BFS to find
 *   multiple shortest augmenting paths simultaneously, and then uses DFS to push flow along them.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Pass the size of the Left partition (N) and Right partition (M).
 *   (1-indexed nodes!).
 *       HopcroftKarp hk(N, M);
 *
 * - Add Edge:
 *       hk.add_edge(u, v); // Worker `u` (1 to N) can do Job `v` (1 to M).
 *
 * - Query:
 *       int max_pairs = hk.max_matching();
 *
 * - Check Pairs:
 *       // After running max_matching(), pair_u[u] gives the job assigned to worker u.
 *       // If pair_u[u] == 0, worker u is unmatched.
 *
 * - Complexity:
 *       Time: O(E * sqrt(V)). Blazing fast for up to 10^5 nodes and edges.
 *       Space: O(V + E) memory.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - König's Theorem (Min Vertex Cover):
 *   To find the minimum number of nodes needed to "cover" all edges:
 *   1. Run Max Matching. Min Vertex Cover = Max Matching.
 *   2. To find the ACTUAL nodes: Run a DFS from all UNMATCHED nodes in the Left partition
 *      using alternating paths. The Vertex Cover = (Unvisited nodes in Left) + (Visited nodes in Right).
 *
 * - Maximum Independent Set:
 *   The maximum number of nodes where no two nodes share an edge.
 *   Formula: Max Independent Set = (Total Nodes) - (Min Vertex Cover).
 */

struct HopcroftKarp
{
    int n; // Size of the Left partition (U)
    int m; // Size of the Right partition (V)

    vector<vector<int>> adj;

    // pair_u[i] = the node in V matched with node i in U
    // pair_v[j] = the node in U matched with node j in V
    vector<int> pair_u, pair_v;

    // distance array used by BFS to find shortest augmenting paths
    vector<int> dist;

    const int INF = 1e9;

    HopcroftKarp(int n, int m) : n(n), m(m)
    {
        adj.resize(n + 1);
        pair_u.assign(n + 1, 0);
        pair_v.assign(m + 1, 0);
        dist.assign(n + 1, 0);
    }

    void add_edge(int u, int v)
    {
        adj[u].push_back(v); // u is in Left (1...N), v is in Right (1...M)
    }

    // BFS builds the level graph, finding the shortest paths to unmatched nodes in V
    bool bfs()
    {
        queue<int> q;

        for (int u = 1; u <= n; u++)
        {
            // If u is unmatched, it's a starting point for an augmenting path
            if (pair_u[u] == 0)
            {
                dist[u] = 0;
                q.push(u);
            }
            else
            {
                dist[u] = INF; // Otherwise, mark as unvisited
            }
        }

        // Node 0 acts as a dummy node for all unmatched nodes in V
        dist[0] = INF;

        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            // If this node has a distance less than the dummy node, we can explore it
            if (dist[u] < dist[0])
            {
                for (int v : adj[u])
                {
                    // If the node matched to v is unvisited, we can walk there
                    if (dist[pair_v[v]] == INF)
                    {
                        dist[pair_v[v]] = dist[u] + 1;
                        q.push(pair_v[v]);
                    }
                }
            }
        }

        // If dist[0] is no longer INF, we found at least one augmenting path!
        return dist[0] != INF;
    }

    // DFS greedily pushes "flow" along the paths discovered by BFS
    bool dfs(int u)
    {
        if (u != 0)
        {
            for (int v : adj[u])
            {
                // Ensure we only walk along the shortest path DAG built by BFS
                if (dist[pair_v[v]] == dist[u] + 1)
                {
                    if (dfs(pair_v[v]))
                    {
                        // Augment the path! Flip the matching status.
                        pair_v[v] = u;
                        pair_u[u] = v;
                        return true;
                    }
                }
            }
            // If no augmenting path could be found from u, mark it as dead end
            dist[u] = INF;
            return false;
        }
        // Base case: Reached the dummy node 0 (an unmatched node in V)
        return true;
    }

    // Repeatedly runs BFS and DFS to build the maximum matching
    int max_matching()
    {
        int matching = 0;

        // While there is an augmenting path...
        while (bfs())
        {
            // Try to augment from every unmatched node in U
            for (int u = 1; u <= n; u++)
            {
                if (pair_u[u] == 0 && dfs(u))
                {
                    matching++;
                }
            }
        }
        return matching;
    }
};

void solve()
{
    int n, m, edges;
    // Example: N workers, M jobs, and E compatible worker-job pairs
    if (!(cin >> n >> m >> edges))
        return;

    HopcroftKarp hk(n, m);

    for (int i = 0; i < edges; i++)
    {
        int u, v;
        cin >> u >> v;
        hk.add_edge(u, v);
    }

    cout << "Maximum Pairs: " << hk.max_matching() << "\n";

    // Print the actual assignments
    for (int u = 1; u <= n; u++)
    {
        if (hk.pair_u[u] != 0)
        {
            cout << "Worker " << u << " is assigned Job " << hk.pair_u[u] << "\n";
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