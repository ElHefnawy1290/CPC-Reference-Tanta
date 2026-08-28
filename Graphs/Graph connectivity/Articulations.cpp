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
