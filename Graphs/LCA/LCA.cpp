struct LCA
{
    int n, LG, SKIP;
    vector<vector<int>> T, Cost;
    vector<int> par, dep;

    LCA(int nodes_count, const GRAPH &graph, int root = 1, int skip_val = OO)
    {
        n = nodes_count;
        LG = __lg(n) + 2;
        SKIP = skip_val;
        T.assign(LG, vector<int>(n + 1, -1));
        Cost.assign(LG, vector<int>(n + 1, SKIP));
        par.assign(n + 1, -1);
        dep.assign(n + 1, 0);

        Cost[0][root] = SKIP;
        dfs(root, -1, 0, graph);

        for (int i = 1; i <= n; i++)
            T[0][i] = par[i];

        for (int pw = 1; (1 << pw) <= n; pw++)
        {
            for (int i = 1; i <= n; i++)
            {
                if (T[pw - 1][i] != -1)
                {
                    T[pw][i] = T[pw - 1][T[pw - 1][i]];
                    Cost[pw][i] = merge(Cost[pw - 1][i], Cost[pw - 1][T[pw - 1][i]]);
                }
            }
        }
    }

    int merge(int a, int b)
    {
        return min(a, b);
    }

    void dfs(int node, int p, int d, const GRAPH &graph)
    {
        dep[node] = d;
        par[node] = p;
        for (auto &[i, w] : graph[node])
        {
            if (i != p)
            {
                Cost[0][i] = w;
                dfs(i, node, d + 1, graph);
            }
        }
    }

    int get_kth(int x, int k)
    {
        while (k)
        {
            if (x == -1)
                break;
            int pw = __builtin_ctz(k);
            x = T[pw][x];
            k &= (k - 1);
        }
        return x;
    }

    int get_lca(int u, int v)
    {
        if (dep[u] < dep[v])
            swap(u, v);
        u = get_kth(u, dep[u] - dep[v]);
        if (u == v)
            return u;
        for (int j = LG - 1; j >= 0; j--)
        {
            if (T[j][u] != T[j][v])
            {
                u = T[j][u];
                v = T[j][v];
            }
        }
        return par[u];
    }

    int get_cost(int u, int v)
    {
        if (dep[u] < dep[v])
            swap(u, v);
        int k = dep[u] - dep[v], x = u;
        int cost = SKIP;
        while (k)
        {
            if (x == -1)
                break;
            int pw = __builtin_ctz(k);
            cost = merge(cost, Cost[pw][x]);
            x = T[pw][x];
            k &= (k - 1);
        }
        return cost;
    }

    int query_path(int u, int v)
    {
        int lca = get_lca(u, v);
        return merge(get_cost(u, lca), get_cost(v, lca));
    }

    int get_dist(int u, int v)
    {
        int lca = get_lca(u, v);
        return dep[u] + dep[v] - 2 * dep[lca];
    }
};
