#include <bits/stdc++.h>
using namespace std;

/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: BEST THEOREM
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Count Eulerian Circuits", "Directed Graph Cycles".
 * - Classic Scenarios: You have a directed graph where every node's in-degree equals
 *   its out-degree, and you need to know how many distinct Eulerian circuits it has.
 * - The Magic: The number of Eulerian circuits = t_w(G) * prod((deg(v) - 1)!)
 *   where t_w(G) is the number of arborescences rooted at any vertex w.
 */

const int MOD = 1e9 + 7;

// Gaussian Elimination for Determinant (MOD)
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

long long modInverse(long long n) { return power(n, MOD - 2); }

long long determinant(vector<vector<long long>> &mat, int n)
{
    long long det = 1;
    for (int i = 0; i < n; i++)
    {
        int pivot = i;
        for (int j = i + 1; j < n; j++)
            if (abs(mat[j][i]) > abs(mat[pivot][i]))
                pivot = j;
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
                mat[j][k] = (mat[j][k] - factor * mat[i][k] % MOD + MOD) % MOD;
        }
    }
    return det;
}

long long best_theorem(int n, vector<vector<int>> &adj)
{
    vector<int> out_degree(n, 0);
    vector<vector<long long>> L(n, vector<long long>(n, 0));

    // 1. Build Directed Laplacian
    for (int u = 0; u < n; ++u)
    {
        for (int v : adj[u])
        {
            out_degree[u]++;
            L[u][v]--;
        }
    }
    for (int i = 0; i < n; ++i)
        L[i][i] += out_degree[i];

    // 2. Compute arborescences (t_w)
    // Delete row 0 and col 0 (root at 0)
    vector<vector<long long>> minor(n - 1, vector<long long>(n - 1));
    for (int i = 1; i < n; i++)
        for (int j = 1; j < n; j++)
            minor[i - 1][j - 1] = (L[i][j] % MOD + MOD) % MOD;

    long long t_w = determinant(minor, n - 1);

    // 3. Multiply by prod((deg(v) - 1)!)
    long long fact_prod = 1;
    vector<long long> fact(n + 1, 1);
    for (int i = 2; i <= n; i++)
        fact[i] = (fact[i - 1] * i) % MOD;

    for (int i = 0; i < n; i++)
    {
        if (out_degree[i] > 0)
            fact_prod = (fact_prod * fact[out_degree[i] - 1]) % MOD;
    }

    return (t_w * fact_prod) % MOD;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    // int n; cin >> n; ... adj list setup ...
    return 0;
}