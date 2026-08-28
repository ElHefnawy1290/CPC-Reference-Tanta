#include <bits/stdc++.h>
using namespace std;
#define ll long long


// Compute 2D prefix sum
void computePrefixSum(const vector<vector<int>>& a, vector<vector<long long>>& prefix) {
    int n = a.size(), m = a[0].size();
    prefix.assign(n + 1, vector<long long>(m + 1, 0));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            prefix[i][j] = a[i - 1][j - 1]
                         + prefix[i - 1][j]
                         + prefix[i][j - 1]
                         - prefix[i - 1][j - 1];
        }
    }
}

// Query sum of submatrix from (r1, c1) to (r2, c2), inclusive (0-based indexing)
long long getSum(const vector<vector<long long>>& prefix, int r1, int c1, int r2, int c2) {
    r1++; c1++; r2++; c2++; // convert to 1-based for prefix array
    return prefix[r2][c2]
         - prefix[r1 - 1][c2]
         - prefix[r2][c1 - 1]
         + prefix[r1 - 1][c1 - 1];
}


void solve() {
    int n , m , k; cin >> n >> m >> k;
    vector<vector<int>> mat(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> mat[i][j];
        }
    }


    vector<vector<ll>>pre;
    computePrefixSum(mat, pre);



}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    cin >> t;
    while (t--)solve();
    return 0;
}