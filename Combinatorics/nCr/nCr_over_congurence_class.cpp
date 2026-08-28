/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: nCr OVER CONGRUENCE CLASS
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Sum of nCr where r mod M = K".
 * - Classic Scenarios: You need to evaluate sum(nCr(N, i)) for all i where
 *   i % M == K. N can be up to 10^18, but M is small (M <= 100).
 * - The Magic: It maps the combinatorial states into a matrix. If you have an
 *   array tracking the sum mod M, multiplying it by a transition matrix simulates
 *   increasing N by 1. By exponentiating the matrix to the power of N in
 *   O(M^3 log N), you instantly compute the exact sum across the entire boundary.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       long long ans = nCr_congruence(N, K, M, MOD);
 *
 * - Complexity:
 *       Time: O(M^3 * log N)
 *       Space: O(M^2) for the matrix.
 */

using ll = long long;

using Matrix = vector<vector<ll>>;

Matrix multiply_matrix(const Matrix &A, const Matrix &B, ll mod)
{
    int n = A.size();
    Matrix C(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < n; k++)
        {
            for (int j = 0; j < n; j++)
            {
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % mod;
            }
        }
    }
    return C;
}

Matrix power_matrix(Matrix A, ll exp, ll mod)
{
    int n = A.size();
    Matrix res(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++)
        res[i][i] = 1;
    while (exp > 0)
    {
        if (exp % 2 == 1)
            res = multiply_matrix(res, A, mod);
        A = multiply_matrix(A, A, mod);
        exp /= 2;
    }
    return res;
}

ll nCr_congruence(ll n, int k, int m, ll mod)
{
    if (m == 1)
        return custom_pow(2, n, mod);
    Matrix T(m, vector<ll>(m, 0));
    for (int i = 0; i < m; i++)
    {
        T[i][i] = 1;
        T[i][(i + m - 1) % m] = 1;
    }
    T = power_matrix(T, n, mod);
    return T[k][0];
}