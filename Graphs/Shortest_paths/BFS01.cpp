#include <bits/stdc++.h>
using namespace std;

void zero_one_bfs(int start, int n, vector<vector<pair<int, int>>> &adj, vector<int> &dist)
{
    dist.assign(n, 1e9);
    deque<int> dq;
    dist[start] = 0;
    dq.push_front(start);

    while (!dq.empty())
    {
        int u = dq.front();
        dq.pop_front();

        for (auto &edge : adj[u])
        {
            int v = edge.first;
            int w = edge.second;
            if (dist[v] > dist[u] + w)
            {
                dist[v] = dist[u] + w;
                if (w == 0)
                    dq.push_front(v);
                else
                    dq.push_back(v);
            }
        }
    }
}