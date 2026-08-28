const int N = 1e5 + 5;
GRAPH graph(N);
int tin[N], tout[N], value[N], flat[N];
int timer{1};

void dfs(int node, int p)
{
    flat[timer] = value[node];
    tin[node] = timer++;
    for (auto &x : graph[node])
    {
        if (x != p)
            dfs(x, node);
    }
    tout[node] = timer;
    // [tin, tout)
}
