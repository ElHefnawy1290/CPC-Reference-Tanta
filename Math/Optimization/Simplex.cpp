/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: SIMPLEX ALGORITHM
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Maximize profit", "Minimize cost", "Subject to constraints".
 * - Classic Scenarios: A generic resource allocation problem where you need to
 *   maximize an objective function C^T X subject to a system of inequalities AX <= B.
 * - The Magic: Bypasses the need for complex min-cost max-flow graph modeling
 *   or impossible greedy logic. You literally just pass the coefficients of your
 *   equations into the matrix.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       vector<double> ans_vars;
 *       double max_profit = simplex(A, b, c, ans_vars);
 * - Result Breakdown:
 *       Returns numeric_limits<double>::infinity() if the problem is unbounded.
 *       Returns -numeric_limits<double>::infinity() if there is no feasible solution.
 *       Otherwise, returns the maximum possible value of the objective function,
 *       and populates `ans_vars` with the optimal values for variables X_1 to X_n.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Handling >= constraints: If an equation is 5x + 3y >= 10, multiply the entire
 *   row by -1 to make it -5x - 3y <= -10 before pushing it to matrix A and vector b.
 * - Handling == constraints: Split it into two inequalities:
 *   (5x + 3y <= 10) AND (-5x - 3y <= -10).
 */

const double EPS = 1e-9;
const double INF = numeric_limits<double>::infinity();

double simplex(vector<vector<double>> A, vector<double> b, vector<double> c, vector<double> &x)
{
    int m = b.size(), n = c.size();
    vector<int> N(n + 1), B(m);
    vector<vector<double>> D(m + 2, vector<double>(n + 2));

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
            D[i][j] = A[i][j];
        B[i] = n + i;
        D[i][n] = -1;
        D[i][n + 1] = b[i];
    }
    for (int j = 0; j < n; j++)
    {
        N[j] = j;
        D[m][j] = -c[j];
    }
    N[n] = -1;
    D[m + 1][n] = 1;

    auto pivot = [&](int r, int s)
    {
        double inv = 1.0 / D[r][s];
        for (int i = 0; i < m + 2; i++)
        {
            if (i != r && abs(D[i][s]) > EPS)
            {
                double inv2 = D[i][s] * inv;
                for (int j = 0; j < n + 2; j++)
                {
                    D[i][j] -= D[r][j] * inv2;
                }
                D[i][s] = D[r][s] * inv2;
            }
        }
        for (int j = 0; j < n + 2; j++)
        {
            if (j != s)
                D[r][j] *= inv;
        }
        D[r][s] = inv;
        swap(B[r], N[s]);
    };

    auto phase = [&](int row)
    {
        while (true)
        {
            int s = -1;
            for (int j = 0; j <= n; j++)
            {
                if (N[j] != -1 && (s == -1 || D[row][j] < D[row][s] - EPS))
                {
                    s = j;
                }
            }
            if (D[row][s] > -EPS)
                return true;

            int r = -1;
            for (int i = 0; i < m; i++)
            {
                if (D[i][s] > EPS && (r == -1 || D[i][n + 1] / D[i][s] < D[r][n + 1] / D[r][s] - EPS))
                {
                    r = i;
                }
            }
            if (r == -1)
                return false;
            pivot(r, s);
        }
    };

    int r = 0;
    for (int i = 1; i < m; i++)
    {
        if (D[i][n + 1] < D[r][n + 1])
            r = i;
    }

    if (D[r][n + 1] < -EPS)
    {
        pivot(r, n);
        if (!phase(m + 1) || D[m + 1][n + 1] < -EPS)
            return -INF;
        for (int i = 0; i < m; i++)
        {
            if (B[i] == -1)
            {
                int s = -1;
                for (int j = 0; j <= n; j++)
                {
                    if (N[j] != -1 && (s == -1 || D[i][j] < D[i][s] - EPS))
                        s = j;
                }
                pivot(i, s);
            }
        }
    }

    if (!phase(m))
        return INF;

    x.assign(n, 0);
    for (int i = 0; i < m; i++)
    {
        if (B[i] < n)
            x[B[i]] = D[i][n + 1];
    }

    return D[m][n + 1];
}