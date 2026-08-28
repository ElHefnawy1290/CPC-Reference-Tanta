class DSU {
public:
    vector<int>parent,sizes,mn,mx;
    DSU(int n) {
        parent.resize(n+1);
        sizes.resize(n+1);
        mx.resize(n+1);
        mn.resize(n+1);
        for (int i = 1; i <= n; i++)
            parent[i] = mn[i] = mx[i] = i,sizes[i] = 1  ;
    }

    int findRoot(int u ) {
        return parent[u] == u ? u : parent[u] = findRoot(parent[u]);
    }

    bool merge(int u, int v) {
        int root_u = findRoot(u), root_v = findRoot(v);
        if (root_u == root_v)
            return false;

        if (sizes[root_u] > sizes[root_v])
            swap(root_u, root_v);

        mx[root_v] = max(mx[root_v], mx[root_u]);
        mn[root_v] = min(mn[root_v], mn[root_u]);
        parent[root_u] = root_v;
        sizes[root_v] += sizes[root_u];
        return true;
    }
};

void Hassan() {
    int n,q;cin >>n>>q;
    DSU d(n);
    while (q--) {
        string op;int u,v;cin >> op ;
        if (op == "get") {
            cin >>v;
            int r = d.findRoot(v);
            cout << d.mn[r] <<" " << d.mx[r] <<" "<< d.sizes[r] <<endl;
        }else {
            cin >>u>>v;
            d.merge(u,v);
        }
    }
}
