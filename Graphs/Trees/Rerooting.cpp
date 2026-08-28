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
