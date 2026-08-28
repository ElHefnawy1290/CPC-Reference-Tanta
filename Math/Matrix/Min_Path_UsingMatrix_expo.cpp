* 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: MIN-PLUS MATRIX EXPONENTIATION 
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Shortest path of exactly K edges", "Paths of length K", "K is up to 10^9 or 10^18", "Min-Plus Algebra".
 * - Classic Scenarios: You are given a graph and need to find the minimum cost to travel between 
 *   nodes using EXACTLY (or at most) K edges. Since K is huge, standard algorithms like Dijkstra 
 *   or Bellman-Ford will Time Limit Exceed (TLE).
 * - The Magic: We treat the adjacency matrix as paths of length 1. If we multiply the matrix by itself, 
 *   we get paths of length 2. But instead of standard multiplication (+ and *), we use the 
 *   Min-Plus Semiring (min instead of +, and + instead of *). 
 *   Using binary exponentiation, we can compute A^K to find paths of length K in just O(N^3 log K)!
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Create an N x N matrix filled with infinity (`oo`).
 *       Matrix t(n, Row(n, oo));
 * - Build Base State: Set the weight of directed edges. 
 *       t[u][v] = w; // Cost of 1 edge from u to v
 * - Execution: Call `fastPower` to raise the matrix to the power of K.
 *       t = fastPower(t, k);


#define Row vector<int>
#define Matrix vector<Row>
const int mod = 1e9 + 7;
const int oo = 2e18+5;

Matrix mul(Matrix &a,Matrix &b) {
    int n = a.size();
    int m = b[0].size();
    int sz = a[0].size();
    Matrix res(n,Row(m,oo));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int o = 0; o < sz; o++) {
                res[i][j] = min(res[i][j],a[i][o] + b[o][j]);
            }
        }
    }
    return res;
}

Matrix fastPower(Matrix &a,int b) {
    int n = a.size();
 
    Matrix res(n, Row(n, oo));
    for (int i = 0; i < n; i++) {
        res[i][i] = 0; 
    }
    
    while (b) {
        if (b & 1)
            res = mul(res, a);
        a = mul(a, a);
        b /= 2;
    }
    return res;
}

void solve() {
    int n, m, k;
    cin >> n >> m >> k;
    Matrix t(n,Row(n,oo));
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        --u;
        --v;
        t[u][v] = w;
    }
    t = fastPower(t,k);

    int ans = oo;
    for (int i = 0; i < n; i++) {
        for (int j=0;j<n;j++) {
            ans = min(ans, t[i][j]);
        }
    }
    if (ans > 1e18) {
        cout << "IMPOSSIBLE\n";
    } else {
        cout << ans << "\n";
    }
}
