ll n;
GRAPH graph;

vector<ll> dijkstra(ll x)
{
    vector<ll> dist(n + 1, OO);
    dist[x] = 0;
    priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pair<ll, ll>>> pq;
    pq.push({0, x});
    while (!pq.empty())
    {
        auto [l, node] = pq.top();
        pq.pop();
        if (l > dist[node])
            continue;
        for (auto &[to, w] : graph[node])
        {
            if (dist[to] > dist[node] + w)
            {
                dist[to] = dist[node] + w;
                pq.push({dist[to], to});
            }
        }
    }
    return dist;
}


// Path
ll n;
GRAPH graph;
vector<int> parent;
deque<int> path;

vector<ll> dijkstra(ll x)
{
    parent.assign(n + 1, -1);
    vector<ll> dist(n + 1, OO);
    dist[x] = 0;
    priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pair<ll, ll>>> pq;
    pq.push({0, x});
    while (!pq.empty())
    {
        auto [l, node] = pq.top();
        pq.pop();
        if (l > dist[node])
            continue;
        for (auto &[to, w] : graph[node])
        {
            if (dist[to] > dist[node] + w)
            {
                dist[to] = dist[node] + w;
                parent[to] = node;
                pq.push({dist[to], to});
            }
        }
    }
    return dist;
}

void build(int node)
{
    path.push_front(node);
    if (parent[node] == -1)
        return;
    build(parent[node]);
}

// Destination
ll n;
GRAPH graph;

vector<ll> dijkstra(ll x, ll d)
{
    vector<ll> dist(n + 1, OO);
    dist[x] = 0;
    priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pair<ll, ll>>> pq;
    pq.push({0, x});
    while (!pq.empty())
    {
        auto [l, node] = pq.top();
        pq.pop();
        if (node == d)
            break;
        if (l > dist[node])
            continue;
        for (auto &[to, w] : graph[node])
        {
            if (dist[to] > dist[node] + w)
            {
                dist[to] = dist[node] + w;
                pq.push({dist[to], to});
            }
        }
    }
    return dist;
}


// Dijkstra indexed PQ
#include <bits/stdc++.h>

#include <ext/pb_ds/priority_queue.hpp>
using namespace std;

using namespace __gnu_pbds;
typedef long long ll;
const ll OO = 1e18;
typedef vector<vector<pair<ll, ll>>> GRAPH;
typedef __gnu_pbds::priority_queue<pair<ll, ll>, greater<pair<ll, ll>>, pairing_heap_tag> indexedPQ;

ll n;
GRAPH graph;

vector<ll> dijkstra(ll x)
{
    vector<ll> dist(n + 1, OO);
    indexedPQ pq;
    vector<indexedPQ::point_iterator> key(n + 1, nullptr);
    dist[x] = 0;
    key[x] = pq.push({0, x});
    while (!pq.empty())
    {
        auto [l, node] = pq.top();
        pq.pop();

        for (auto &[to, w] : graph[node])
        {
            if (dist[to] > dist[node] + w)
            {
                dist[to] = dist[node] + w;
                if (key[to] == nullptr)
                    key[to] = pq.push({dist[to], to});
                else
                    pq.modify(key[to], {dist[to], to});
            }
        }
    }
    return dist;
}

// Dijkstra remove
ll n;
GRAPH graph;

vector<ll> dijkstra(ll x)
{
    vector<ll> dist(n + 1, OO);
    dist[x] = 0;
    set<pair<ll, ll>> s;
    s.insert({0, x});
    while (!s.empty())
    {
        auto [l, node] = *s.begin();
        s.erase(s.begin());
        if (l > dist[node])
            continue;
        for (auto &[to, w] : graph[node])
        {
            if (dist[to] > dist[node] + w)
            {
                dist[to] = dist[node] + w;
                s.insert({dist[to], to});
            }
        }
    }
    return dist;
}
