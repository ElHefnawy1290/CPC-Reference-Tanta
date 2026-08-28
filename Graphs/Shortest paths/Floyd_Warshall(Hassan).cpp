void hassan() {
    int n,m;cin >>n>>m;
    vector<vector<int>> dist(n+1, vector<int>(n+1, oo)); // oo >= n * maxCost

    for (int i=0;i<m;i++) {
        int u,v,w;cin>>u>>v>>w;
        dist[u][v] = min(dist[u][v], w);
    }
    for (int i=1;i<=n;i++) {
        dist[i][i] = 0;
    }

    for (int k = 1; k <= n; k++) {
        for (int from = 1; from <= n; from++) {
            for (int to = 1; to <= n; to++) {
                if (dist[from][k] < oo && dist[k][to] < oo) {
                    dist[from][to] = min(dist[from][to], dist[from][k] + dist[k][to]);
                }
            }
        }

    for (int i=1;i<=n;i++) {
        for (int j=1;j<=n;j++) {
            cout << "from " << i << " to " << j << " Dist " <<dist[i][j] << "\n";
        }
    }
}

void reachability(int n, int m) {
    vector<vector<bool>> canReach(n+1, vector<bool>(n+1, false));
    // ... input u, v ... canReach[u][v] = true;
    for (int i=1; i<=n; i++) canReach[i][i] = true;

    for (int k=1; k<=n; k++) {
        for (int i=1; i<=n; i++) {
            for (int j=1; j<=n; j++) {     
                canReach[i][j] = canReach[i][j] || (canReach[i][k] && canReach[k][j]);
            }
        }
    }
}

///  Find Minimax Or Maxmini Path ///////
void hassan() {
    int n,m;cin >>n>>m;
    vector<vector<int>> dist(n+1, vector<int>(n+1, oo)); // oo >= n * maxCost
    for (int i=1;i<=n;i++) {
        dist[i][i] = 0;
     }
    for (int i=0;i<m;i++) {
        int u,v,w;cin>>u>>v>>w;
        dist[u][v] = min(dist[u][v], w);
    }
  
    for (int k = 1; k <= n; k++) {
        for (int from = 1; from <= n; from++) {
            for (int to = 1; to <= n; to++) {
                if (dist[from][k] < oo && dist[k][to] < oo) {
                    dist[from][to] = min(dist[from][to], max(dist[from][k],dist[k][to]));
                    //dist[from][to] = max(dist[from][to], min(dist[from][k],dist[k][to]));
                }
            }
        }
        for (int i=1;i<=n;i++) {
            for (int j=1;j<=n;j++) {
                cout << "from " << i << " to " << j << " Dist " <<dist[i][j] << "\n";
            }
        }
    }
}

///// Counting Shortest Paths ///////
void hassan_counting() {
    int n, m; cin >> n >> m;
    long long oo = 1e15;

    vector<vector<long long>> dist(n + 1, vector<long long>(n + 1, oo));
    vector<vector<long long>> pathsCount(n + 1, vector<long long>(n + 1, 0));

    for (int i = 1; i <= n; i++) {
        dist[i][i] = 0;
        pathsCount[i][i] = 1;   
    }

    for (int i = 0; i < m; i++) {
        int u, v, w; cin >> u >> v >> w;
        if (w < dist[u][v]) {
            dist[u][v] = w;
            pathsCount[u][v] = 1;
        } else if (w == dist[u][v]) {
            pathsCount[u][v]++; 
        }
    }

    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    pathsCount[i][j] = pathsCount[i][k] * pathsCount[k][j];
                } 
                else if (dist[i][k] + dist[k][j] == dist[i][j] && i != k && k != j) {
                    pathsCount[i][j] += (pathsCount[i][k] * pathsCount[k][j]);
                }
            }
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (dist[i][j] == oo) continue;
            cout << "From " << i << " to " << j << ": Min Dist = " << dist[i][j] 
                 << " | Ways = " << pathsCount[i][j] << "\n";
        }
    }
}


/////// Diameter and Radius  /////////
int diameter = 0;
int radius = oo;

for (int i=1; i<=n; i++) {
    int max_dist_for_i = 0;
    for (int j=1; j<=n; j++) {
        if (dist[i][j] != oo) 
            max_dist_for_i = max(max_dist_for_i, dist[i][j]);
    }
    // Diameter: أطول "أقصر طريق" في الرسم كله
    diameter = max(diameter, max_dist_for_i);
    // Radius: أصغر "أقصى مسافة" من نقطة معينة لباقي النقط
    radius = min(radius, max_dist_for_i);
}
