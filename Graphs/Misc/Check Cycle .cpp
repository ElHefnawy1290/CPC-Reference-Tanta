// Check Cycle in Undirected
Graph graph ;
vector<bool>  vis;
bool isCycle = 0;
void dfs(int u,int prv )
{
    vis[u] = 1;
    for (auto& n : graph[u])
    {
        if (n == prv) continue;
        if (!vis[n])
            dfs(n, u);
        else
            isCycle = 1;
    }
}

// Check Cycle in directed
Graph graph ;
vector<int>  vis;
bool isCycle = 0;
void dfs(int u )
{
    vis[u] = 2;  // mark node that exsit in recurion stack
    for (auto& n : graph[u])
    {
        if (!vis[n])  // if node not vis & not exist inf recursion stack
            dfs(n);
        else if (vis[n] == 2) // node is visited and exsit in recursion stack
            isCycle = 1;
    }

    vis[u] = 1;  // mark node that visited and not exsit in recursion stack

}