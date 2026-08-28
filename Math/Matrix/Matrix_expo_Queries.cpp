
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: FAST PATH COUNTING (PRECOMPUTED MATRIX EXPONENTIATION)
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Number of paths of length K", "Multiple Queries (Q)", "State Vector multiplication".
 * - Classic Scenarios: You need to find the number of valid paths of exactly length K between 
 *   node U and node V in a directed graph, modulo some number. BUT, you have Q queries! 
 *   Standard Matrix Exponentiation per query takes O(Q * N^3 * log K), which will Time Limit Exceed (TLE).
 * - The Magic: "The Vector-Matrix Trick". We precompute the binary powers of the adjacency 
 *   matrix A^(2^0), A^(2^1), ..., A^(2^29) ONCE in O(N^3 * log K). 
 *   Then, for each query, we don't multiply N x N matrices together. Instead, we start with 
 *   a 1 x N state vector (where only the starting node U is 1) and multiply this VECTOR 
 *   by the precomputed N x N matrices. This slashes the query time down to just O(N^2 * log K)!
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Base State: Build your adjacency matrix `t`. If there is a directed edge u -> v, `t[u][v] = 1`.
 * - Precomputation: 
 *       pre[0] = t;
 *       pre[i] = mul(pre[i-1], pre[i-1]); // Computes A^(2^i)
 * - Queries: 
 *       Create a 1 x N matrix `s` (state vector). Set `s[0][u] = 
 *       For each set bit i in K, update `s = mul(s, pre[i])`.
 *       The answer is simply `s[0][v]`.
 *
 * - Complexity:
 *       Time: Precomputation O(N^3 * log K) + Queries O(Q * N^2 * log K).
 *       Space: O(log K * N^2) to store the 30 precomputed matrices.

const int mod = 1e9 + 7;
#define Row vector<int>
#define mat vector<Row>
mat mul(mat &a,mat &b) {
    int n = a.size(), m = b[0].size(),k = a[0].size();
    mat res(n,Row(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int o = 0; o < k; o++) {
                res[i][j] += a[i][o]%mod * b[o][j]%mod;
                res[i][j]%=mod;
            }
        }
    }
    return res;
}

mat power(mat a,int b) {
    int n = a.size();
    mat res(n,Row(n));
    for (int i = 0; i < n; i++) res[i][i] = 1;
    while (b) {
        if (b&1) {
            res = mul(res,a);
        }
        b>>=1;
        a = mul(a,a);
    }
    return res;
}
void solve() {
    int n,m,q;cin >>n>>m>>q;
    mat t(n,Row(n));
    for (int i = 0; i < m; i++) {
        int u,v;cin >>u>>v;
        u--; v--;
        t[u][v] = 1;
    }
    vector<mat> pre(30,mat(n,Row(n)));
    pre[0] = t;
    for (int i = 1; i < 30; i++) {
        pre[i] = mul(pre[i-1],pre[i-1]);
    }
    while (q--) {
        int u,v,k;cin>>u>>v >>k;--u,--v;
        mat s(1,Row(n));
        s[0][u] = 1;
        for (int i = 0; i < 30; i++) {
            if (1 << i & k) {
                s = mul(s,pre[i]);
            }
        }
        cout << s[0][v] <<'\n';
    }
}
