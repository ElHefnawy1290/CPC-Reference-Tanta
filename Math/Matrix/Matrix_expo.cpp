typedef vector<vector<ll>> Matrix;

Matrix matrix_mul(Matrix &A, Matrix &B)
{
    Matrix C;
    ll n = ll(A.size());
    ll m = ll(A[0].size());
    ll l = ll(B[0].size());
    C.resize(n, vector<ll>(l));
    for (ll i = 0; i < n; i++)
    {
        for (ll k = 0; k < l; k++)
        {
            for (ll j = 0; j < m; j++)
            {
                C[i][k] += A[i][j] % MOD * B[j][k] % MOD;
                C[i][k] %= MOD;
            }
        }
    }
    return C;
}

Matrix matrix_expo(Matrix &matrix, ll pow)
{
    ll n = ll(matrix.size());
    Matrix res;
    res.resize(n, vector<ll>(n));
    for (int i = 0; i < n; i++)
        res[i][i] = 1;
    while (pow)
    {
        if (pow & 1)
            res = matrix_mul(matrix, res);
        matrix = matrix_mul(matrix, matrix);
        pow >>= 1;
    }
    return res;
}

Matrix I(int n)
{
    Matrix T(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        T[i][i] = 1;
    return T;
}

// 2d Grid represent as graph
// count paths
short dx[8] = {1,1,-1,-1,2,2,-2,-2};
short dy[8] = {2,-2,2,-2,1,-1,1,-1};
void solve() {
    int k;cin >>k;
    Matrix T(65,Row(65));
    for (int i = 0;i<8;i++) {
        for (int j = 0;j<8;j++) {
            int cur = i*8 + j;
            for (int k = 0;k<8;k++) {
                short ni = i+dx[k];
                short nj = j+dy[k];
                if (ni < 0 || nj < 0 || ni > 7 || nj>7) continue;
                int nCur = ni*8+nj;
                T[nCur][cur] = 1;
            }
        }
    }
    for (int i = 0;i<65;i++) {
        T[i][64] = 1;
    }
    T = fastPower(T,k+1);
    int ans = T[0][64];
    cout << ans;
}
 
