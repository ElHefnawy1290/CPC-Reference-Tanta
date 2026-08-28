/*
 * Problem Description:
 * The goal is to find a path from the source node (Node 1) to all other nodes in the graph
 * such that the Maximum edge weight along this path is Minimized (Minimize the maximum weight).
*/
struct node {
    int u, w;

    bool operator<(const node &b) const {
        return w > b.w;
    }
};

void hassan() {
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int> > > adj(n + 1);
    for (int i = 1; i <= m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    priority_queue<node> pq;
    pq.push({1, 0});
    vector<int> minMax(n + 1, oo);
    minMax[1] = 0;
    while (!pq.empty()) {
        auto [u,w] = pq.top();
        pq.pop();

        if (w > minMax[u]) continue;

        for (auto [v,c]: adj[u]) {
            if (max(minMax[u], c) < minMax[v]) {
                minMax[v] = max(minMax[u], c);
                pq.push({v, minMax[v]});
            }
        }
    }
    for (int i = 1; i <= n; i++) {
        cout << minMax[i] << " ";
    }
}