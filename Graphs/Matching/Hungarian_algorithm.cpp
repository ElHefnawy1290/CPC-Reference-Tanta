#include <bits/stdc++.h>
using namespace std;

/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: HUNGARIAN ALGORITHM
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Optimal Assignment", "Bipartite Weighted Matching", "Min-Cost Perfect Matching".
 * - Classic Scenarios: You have N workers and M jobs. Every worker-job pair has a specific
 *   cost or time to complete. You want to assign exactly one job to each worker such that
 *   the total cost is minimized.
 * - The Magic: While MCMF works, Hungarian runs in a strict, highly-optimized O(N * M^2) time,
 *   making it the undisputed king for dense assignment matrices.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization:
 *       // Create a 1-indexed cost matrix of size (N+1) x (M+1)
 *       // Important: N must be <= M! (Rows <= Cols)
 *       Hungarian solver(n, m, cost_matrix);
 *
 * - Query:
 *       long long min_cost = solver.solve();
 *
 * - Extract Assignments:
 *       // solver.ans[i] will contain the job (1 to M) assigned to worker i.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Maximum Profit Assignment?
 *   If you want to MAXIMIZE the score, simply negate all values in your matrix (cost = -cost)
 *   before passing it in. Then negate the final answer returned by `solve()`.
 * - Forbidden Assignments?
 *   If worker `i` cannot do job `j`, set `cost[i][j] = 1e15` (Infinity).
 */

struct Hungarian
{
    int n, m;
    vector<vector<long long>> a;
    vector<long long> u, v, minv;
    vector<int> p, way, ans;
    const long long INF = 1e15;

    Hungarian(int n, int m, const vector<vector<long long>> &cost_matrix)
    {
        this->n = n;
        this->m = m;
        // 1-based indexing logic
        a = cost_matrix;
        u.assign(n + 1, 0);
        v.assign(m + 1, 0);
        p.assign(m + 1, 0);
        way.assign(m + 1, 0);
        ans.assign(n + 1, 0);
    }

    /* STREAMING_CHUNK: Core O(N * M^2) solver using augmenting paths */
    long long solve()
    {
        // Iterate over every worker (row) to find them a job
        for (int i = 1; i <= n; ++i)
        {
            p[0] = i;
            int j0 = 0; // Dummy column 0

            minv.assign(m + 1, INF);
            vector<bool> used(m + 1, false);

            do
            {
                used[j0] = true;
                int i0 = p[j0], j1 = 0;
                long long delta = INF;

                // Update minv array and find the next column to explore
                for (int j = 1; j <= m; ++j)
                {
                    if (!used[j])
                    {
                        long long cur = a[i0][j] - u[i0] - v[j];
                        if (cur < minv[j])
                        {
                            minv[j] = cur;
                            way[j] = j0;
                        }
                        if (minv[j] < delta)
                        {
                            delta = minv[j];
                            j1 = j;
                        }
                    }
                }

                // Update potentials (dual variables)
                for (int j = 0; j <= m; ++j)
                {
                    if (used[j])
                    {
                        u[p[j]] += delta;
                        v[j] -= delta;
                    }
                    else
                    {
                        minv[j] -= delta;
                    }
                }
                j0 = j1;
            } while (p[j0] != 0); // Stop when we reach an unmatched column

            do
            {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while (j0 != 0);
        }

        // The optimal job assigned to worker `i`
        for (int j = 1; j <= m; ++j)
        {
            if (p[j] != 0)
            {
                ans[p[j]] = j;
            }
        }

        // The final minimum cost is stored negated in the dummy column potential v[0]
        return -v[0];
    }
};

void example_usage()
{
    int n, m;
    // Example: N workers, M jobs (ensure N <= M)
    if (!(cin >> n >> m))
        return;

    // Build the 1-indexed cost matrix
    vector<vector<long long>> cost(n + 1, vector<long long>(m + 1, 0));
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            cin >> cost[i][j];
        }
    }

    Hungarian solver(n, m, cost);
    cout << "Minimum Total Cost: " << solver.solve() << "\n";

    for (int i = 1; i <= n; i++)
    {
        cout << "Worker " << i << " assigned to Job " << solver.ans[i] << "\n";
    }
}