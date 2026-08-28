struct LCA {
    vector<vector<pair<int,int>> > adj;
    vector<vector<int>> up,cost[2];
    vector<int> depth;
    int LOG = 30;
    int ign = 1e9;
    void build(int n, vector<vector<pair<int,int>> > &G, int root = 1) {
        adj = G;
        up = vector<vector<int> >(LOG + 5, vector<int>(n + 5));
        cost[0] = vector<vector<int> >(LOG + 5, vector<int>(n + 5));
        cost[1] = vector<vector<int> >(LOG + 5, vector<int>(n + 5));
        depth = vector<int>(n + 5);
        dfs(root, -1);
    }
    void dfs(int u, int p) {
        for (auto [v,w]: adj[u]) {
            if (v == p) continue;
            depth[v] = depth[u] + 1;

            up[0][v] = u;
            cost[0][0][v] = w;
            cost[1][0][v] = w;
            for (int i = 1; i < LOG; i++) {
                up[i][v] = up[i - 1][up[i - 1][v]];
                cost[0][i][v] = min(cost[0][i - 1][v], cost[0][i - 1][up[i - 1][v]]);
                cost[1][i][v]  = max(cost[1][i - 1][v], cost[1][i - 1][up[i - 1][v]]);

            }
            dfs(v, u);
        }
    }

    int kth_ancestor(int u, int k) {
        for (int i = 0; i < LOG; i++) {
            if (k >> i & 1) {
                u = up[i][u];
            }
        }
        return u;
    }

    int getLca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        int k = depth[u] - depth[v];
        u = kth_ancestor(u, k);
        if (u == v) return u;

        for (int i = LOG - 1; i >= 0; --i) {
            if (up[i][u] != up[i][v]) {
                u = up[i][u];
                v = up[i][v];
            }
        }
        return up[0][v];
    }
    pair<int,int> query(int u,int v) {
        int mn = ign,mx = -ign;
        int lca = getLca(u, v);
        int k = depth[u] - depth[lca];
        for (int i=0;i<LOG;i++) {
            if (k>>i&1) {
                mn = min(mn,cost[0][i][u]);
                mx = max(mx,cost[1][i][u]);
                u = up[i][u];
            }
        }
        k = depth[v] - depth[lca];
        for (int i=0;i<LOG;i++) {
            if (k>>i&1) {
                mn = min(mn,cost[0][i][v]);
                mx = max(mx,cost[1][i][v]);
                v = up[i][v];
            }
        }
        return {mn,mx};
    }


};
