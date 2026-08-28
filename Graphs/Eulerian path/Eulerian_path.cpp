const int N = 1e5, M = 1e5;

struct Edge
{
    int to, id;
};

vector<Edge> graph[N];
bool vis[M];
vector<int> ans;

void dfs(int u)
{
    while ((int)graph[u].size() > 0)
    {
        auto [v, id] = graph[u].back();
        graph[u].pop_back();
        if (!vis[id])
        {
            vis[id] = true;
            dfs(v);
        }
    }
    ans.emplace_back(u); // Eulerian path reversed
}
