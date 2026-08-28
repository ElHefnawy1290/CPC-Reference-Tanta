vector<int> res;

void topo_DFS(int node)
{
    vis[node] = true;
    for (auto &neighbor : graph[node])
    {
        if (!vis[neighbor])
            topo_DFS(neighbor);
    }
    res.push_back(node);
}

// Then reverse

vector<int> topSort()
{
    int n = int(graph.size());
    vector<int> inDegree(n);
    for (int i = 0; i < n; i++)
    {
        for (auto &neighbor : graph[i])
            inDegree[neighbor]++;
    }
    queue<int> ready;
    vector<int> ordering;
    for (int i = 0; i < n; i++)
    {
        if (!inDegree[i])
            ready.push(i);
    }
    while (!ready.empty())
    {
        int node = ready.front();
        ready.pop();
        ordering.push_back(node);
        for (auto &neighbor : graph[node])
        {
            if (!--inDegree[neighbor])
                ready.push(neighbor);
        }
    }
    if ((int)ordering.size() != int(graph.size()))
        ordering.clear();
    return ordering;
}
