int n;
GRAPH graph;

void prim(int node)
{
    int total{};
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.emplace(0, node);
    vector<bool> visited(n + 1, 0);
    vector<int> dist(n + 1, OO);
    dist[node] = 0;
    while (!pq.empty())
    {
        auto [w, node] = pq.top();
        pq.pop();
        if (visited[node])
            continue;
        visited[node] = true;
        total += w;
        for (auto &[to, c] : graph[node])
        {
            if (!visited[to] && dist[to] > c)
            {
                dist[to] = c;
                pq.push({dist[to], to});
            }
        }
    }
}
