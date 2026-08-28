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
    graph.assign(n + 1, {}), graph_reverse.assign(n + 1, {}), vis.assign(n + 1, {}), vis_reverse.assign(n + 1, {}), cond_graph.assign(n + 1, {});
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

// Add this helper to add edges easily
void add_clause(int i, bool val_i, int j, bool val_j)
{
    // i, j are variable indices (1 to n)
    // val_i, val_j are the boolean requirements (true for X, false for !X)

    // Logic: (X or Y) is equivalent to (!X -> Y) AND (!Y -> X)

    // Map to 2*i and 2*i+1
    int u = 2 * i + (val_i ? 0 : 1);
    int v = 2 * j + (val_j ? 0 : 1);

    // !u is u^1, !v is v^1
    graph[u ^ 1].emplace_back(v);
    graph[v ^ 1].emplace_back(u);

    // Keep reversed graph for Kosaraju
    graph_reverse[v].emplace_back(u ^ 1);
    graph_reverse[u].emplace_back(v ^ 1);
}

void solve()
{
    // 1. Read input
    cin >> n >> m;
    // Remember to adjust N to 2*N+1 if N was 1e5

    // 2. Clear graph from your SCC template
    // ... (Use your SCC setup code here)

    for (int k = 0; k < m; k++)
    {
        int i, j;
        bool val_i, val_j;
        // Example: Reading "1 True 2 False" means (X1 or !X2)
        cin >> i >> val_i >> j >> val_j;
        add_clause(i, val_i, j, val_j);
    }

    // 3. Run Kosaraju
    SCC();

    // 4. Check for Impossibility
    vector<int> assignment(n + 1);
    for (int i = 1; i <= n; i++)
    {
        if (SCC_id[2 * i] == SCC_id[2 * i + 1])
        {
            cout << "IMPOSSIBLE\n";
            return;
        }
        // 5. The Assignment Logic (The "Higher ID is True" strategy)
        assignment[i] = (SCC_id[2 * i] > SCC_id[2 * i + 1]);
    }

    // 6. Print result
    cout << "POSSIBLE\n";
    for (int i = 1; i <= n; i++)
    {
        cout << assignment[i] << " ";
    }
}