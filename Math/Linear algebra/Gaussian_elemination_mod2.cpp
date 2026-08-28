/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: GAUSSIAN ELIMINATION MOD 2
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "XOR subset", "Boolean equations", "Lights out game".
 * - Classic Scenarios: Finding if a specific XOR sum can be formed from an array,
 *   or solving Ax = B where all values and operations are modulo 2 (XOR).
 * - The Magic: Replaces standard arrays with `std::bitset`, shrinking the time
 *   constant by a factor of 64. Instead of division and subtraction, it simply
 *   uses XOR.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       bitset<2005> ans;
 *       int solutions = gauss_mod2(matrix, N, M, ans);
 *
 * - Complexity:
 *       Time: O(N^3 / 64)
 *       Space: O(N^2 / 64)
 */

const int MAX_V = 2005;

int gauss_mod2(vector<bitset<MAX_V>> a, int n, int m, bitset<MAX_V> &ans)
{
    vector<int> where(m, -1);
    for (int col = 0, row = 0; col < m && row < n; ++col)
    {
        for (int i = row; i < n; ++i)
        {
            if (a[i][col])
            {
                swap(a[i], a[row]);
                break;
            }
        }
        if (!a[row][col])
            continue;
        where[col] = row;
        for (int i = 0; i < n; ++i)
        {
            if (i != row && a[i][col])
            {
                a[i] ^= a[row];
            }
        }
        ++row;
    }

    ans.reset();
    for (int i = 0; i < m; ++i)
    {
        if (where[i] != -1)
            ans[i] = a[where[i]][m];
    }

    for (int i = 0; i < n; ++i)
    {
        int sum = 0;
        for (int j = 0; j < m; ++j)
        {
            if (ans[j] && a[i][j])
                sum ^= 1;
        }
        if (sum != a[i][m])
            return 0;
    }

    for (int i = 0; i < m; ++i)
    {
        if (where[i] == -1)
            return 2;
    }
    return 1;
}