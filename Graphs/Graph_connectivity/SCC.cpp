const int N = 1e5 + 9;

int SCC_id[N];
GRAPH graph, graph_reverse, cond_graph;
int n, m;
vector<int> order, vis, vis_reverse, order_reverse;

void dfs(int node)
{
    vis[node] = true;
    for (auto &x : graph[node])
    {
        if (!vis[x])
            dfs(x);
    }
    order.emplace_back(node);
}

void dfs_reverse(int node)
{
    vis_reverse[node] = true;
    for (auto &x : graph_reverse[node])
    {
        if (!vis_reverse[x])
            dfs_reverse(x);
    }
    order_reverse.emplace_back(node);
}

void SCC()
{
    graph.assign(n + 1, {}), graph_reverse.assign(n + 1, {}), vis.assign(n + 1, {}), vis_reverse.assign(n + 1, {}),
        cond_graph.assign(n + 1, {});
    for (int i = 0; i < m; i++)
    {
        int x, y;
        cin >> x >> y;
        graph[x].emplace_back(y), graph_reverse[y].emplace_back(x);
    }
    order.clear();
    for (int i = 1; i <= n; i++)
    {
        if (!vis[i])
            dfs(i);
    }
    reverse(order.begin(), order.end());
    vector<vector<int>> scc;
    scc.push_back({});
    for (auto &x : order)
    {
        if (!vis_reverse[x])
        {
            order_reverse.clear();
            dfs_reverse(x);
            scc.emplace_back(order_reverse);
            for (auto &val : order_reverse)
                SCC_id[val] = int(scc.size()) - 1;
        }
    }
    for (int u = 1; u <= n; u++)
    {
        for (int v : graph[u])
        {
            if (SCC_id[u] != SCC_id[v])
                cond_graph[SCC_id[u]].emplace_back(SCC_id[v]);
        }
    }
}
