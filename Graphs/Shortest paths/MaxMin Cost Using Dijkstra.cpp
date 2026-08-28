/*
* Problem Description:
* The goal is to find a path from the source node (Node 1) to all other nodes in the graph
* such that the minimum edge weight along this path is maximized (Maximize the minimum weight).
*/
struct node {
    int u,w;

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
    vector<int> maxMin(n + 1, -1);
    priority_queue<node> pq;
    pq.push({1, oo});
    maxMin[1] = oo;
    while (!pq.empty()) {
        auto [u,w] = pq.top();
        pq.pop();

        if (w < maxMin[u]) continue;

        for (auto [v,c]: adj[u]) {
            if (min(maxMin[u], c) > maxMin[v]) {
                maxMin[v] = min(maxMin[u], c);
                pq.push({v, maxMin[v]});
            }
        }
    }
    for (int i = 1; i <= n; i++) {
        cout << maxMin[i] << " ";
    }
}