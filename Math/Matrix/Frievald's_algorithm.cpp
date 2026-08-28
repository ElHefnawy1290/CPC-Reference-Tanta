/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: FREIVALD'S ALGORITHM
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Verify Matrix Multiplication", "A * B = C".
 * - Classic Scenarios: You are given three massive N x N matrices (A, B, and C).
 *   You need to determine if A * B equals C without TLEing.
 * - The Magic: A full multiplication takes O(N^3). Freivald's generates a random
 *   1D vector (r) of 0s and 1s, and checks if A * (B * r) == C * r.
 *   Since matrix-vector multiplication is O(N^2), this is drastically faster.
 *   Repeating it K times drops the probability of a false positive to (1/2)^K.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       bool is_correct = freivalds(A, B, C, 15); // 15 iterations is perfectly safe
 *
 * - Complexity:
 *       Time: O(K * N^2)
 *       Space: O(N)
 */

#include <random>
#include <chrono>

using ll = long long;
const ll MOD = 1e9 + 7;

vector<ll> multiply_mat_vec(const vector<vector<ll>> &mat, const vector<ll> &vec)
{
    int n = mat.size();
    vector<ll> res(n, 0);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            res[i] = (res[i] + mat[i][j] * vec[j]) % MOD;
        }
    }
    return res;
}

bool freivalds(const vector<vector<ll>> &a, const vector<vector<ll>> &b, const vector<vector<ll>> &c, int k_iterations = 15)
{
    int n = a.size();
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dist(0, 1);

    for (int k = 0; k < k_iterations; k++)
    {
        vector<ll> r(n);
        for (int i = 0; i < n; i++)
        {
            r[i] = dist(rng);
        }

        vector<ll> br = multiply_mat_vec(b, r);
        vector<ll> abr = multiply_mat_vec(a, br);
        vector<ll> cr = multiply_mat_vec(c, r);

        for (int i = 0; i < n; i++)
        {
            if (abr[i] != cr[i])
            {
                return false;
            }
        }
    }
    return true;
}