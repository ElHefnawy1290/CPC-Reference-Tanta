#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

struct Edge
{
    int u, v, w;
};

bool bellman_ford(int n, int start, vector<Edge> &edges, vector<long long> &dist)
{
    dist.assign(n, INF);
    dist[start] = 0;

    for (int i = 0; i < n - 1; ++i)
    {
        for (auto &e : edges)
        {
            if (dist[e.u] != INF && dist[e.v] > dist[e.u] + e.w)
                dist[e.v] = dist[e.u] + e.w;
        }
    }

    for (auto &e : edges)
    {
        if (dist[e.u] != INF && dist[e.v] > dist[e.u] + e.w)
            return true;
    }
    return false;
}