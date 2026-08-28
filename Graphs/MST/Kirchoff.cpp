#include <bits/stdc++.h>
using namespace std;

/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: KIRCHHOFF'S THEOREM
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Count spanning trees", "Matrix Tree Theorem".
 * - Classic Scenarios: You have a graph and need to know exactly how many different
 *   spanning trees can be formed.
 * - The Magic: It converts a graph problem into a linear algebra problem.
 *   It constructs the Laplacian Matrix and computes a determinant using
 *   Gaussian Elimination.
 *
 * 2. 📦 HOW TO USE IT
 * - Build the Laplacian matrix (L = Degree - Adjacency).
 * - Remove the last row and last column.
 * - Compute the determinant modulo MOD.
 * - Complexity: O(N^3) (due to Gaussian Elimination).
 */

const int MOD = 1e9 + 7;

long long power(long long base, long long exp)
{
    long long res = 1;
    base %= MOD;
    while (exp > 0)
    {
        if (exp % 2 == 1)
            res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

long long modInverse(long long n)
{
    return power(n, MOD - 2);
}

// ⚙️ Gaussian Elimination to find Determinant
long long determinant(vector<vector<long long>> &mat, int n)
{
    long long det = 1;
    for (int i = 0; i < n; i++)
    {
        int pivot = i;
        for (int j = i + 1; j < n; j++)
        {
            if (abs(mat[j][i]) > abs(mat[pivot][i]))
                pivot = j;
        }
        swap(mat[i], mat[pivot]);
        if (i != pivot)
            det = (MOD - det) % MOD;
        if (mat[i][i] == 0)
            return 0;

        det = (det * mat[i][i]) % MOD;
        long long inv = modInverse(mat[i][i]);
        for (int j = i + 1; j < n; j++)
        {
            long long factor = (mat[j][i] * inv) % MOD;
            for (int k = i + 1; k < n; k++)
            {
                mat[j][k] = (mat[j][k] - factor * mat[i][k] % MOD + MOD) % MOD;
            }
        }
    }
    return det;
}

void solve()
{
    int n, m;
    cin >> n >> m;

    // Laplacian Matrix
    vector<vector<long long>> L(n, vector<long long>(n, 0));

    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        --u;
        --v; // 0-indexed

        L[u][u]++;
        L[v][v]++;
        L[u][v]--;
        L[v][u]--;
    }

    // Convert to positive modulo
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            L[i][j] = (L[i][j] % MOD + MOD) % MOD;

    // Remove last row and column to get the Minor
    vector<vector<long long>> minor(n - 1, vector<long long>(n - 1));
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1; j++)
        {
            minor[i][j] = L[i][j];
        }
    }

    cout << determinant(minor, n - 1) << "\n";
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}