/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: GAUSSIAN ELIMINATION
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "System of equations", "Probabilities", "Markov Chains".
 * - Classic Scenarios: Solving Ax = B for floating-point values.
 * - The Magic: It reduces the matrix to row-echelon form to find the exact
 *   values of the variables.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       vector<double> ans;
 *       int solutions = gauss(matrix, ans);
 * - Result Breakdown:
 *       Returns 1 if there is exactly one solution (stored in 'ans').
 *       Returns 0 if there are no solutions.
 *       Returns 2 if there are infinitely many solutions.
 *
 * - Complexity:
 *       Time: O(N^3)
 *       Space: O(N^2)
 */

const double EPS = 1e-9;

int gauss(vector<vector<double>> a, vector<double> &ans)
{
    int n = a.size();
    if (n == 0)
        return 0;
    int m = a[0].size() - 1;
    vector<int> where(m, -1);

    for (int col = 0, row = 0; col < m && row < n; ++col)
    {
        int sel = row;
        for (int i = row; i < n; ++i)
        {
            if (abs(a[i][col]) > abs(a[sel][col]))
                sel = i;
        }
        if (abs(a[sel][col]) < EPS)
            continue;

        for (int i = col; i <= m; ++i)
            swap(a[sel][i], a[row][i]);
        where[col] = row;

        for (int i = 0; i < n; ++i)
        {
            if (i != row)
            {
                double c = a[i][col] / a[row][col];
                for (int j = col; j <= m; ++j)
                {
                    a[i][j] -= a[row][j] * c;
                }
            }
        }
        ++row;
    }

    ans.assign(m, 0);
    for (int i = 0; i < m; ++i)
    {
        if (where[i] != -1)
            ans[i] = a[where[i]][m] / a[where[i]][i];
    }

    for (int i = 0; i < n; ++i)
    {
        double sum = 0;
        for (int j = 0; j < m; ++j)
        {
            sum += ans[j] * a[i][j];
        }
        if (abs(sum - a[i][m]) > EPS)
            return 0;
    }

    for (int i = 0; i < m; ++i)
    {
        if (where[i] == -1)
            return 2;
    }
    return 1;
}