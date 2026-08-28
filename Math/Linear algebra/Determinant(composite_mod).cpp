/**
 * 🎯 DETERMINANT WITH COMPOSITE MODULO
 *
 * WHAT PROBLEM DOES THIS SOLVE?
 * Standard matrix determinants require division (modular inverse). If the modulo
 * is composite, the modular inverse might not exist. This algorithm bypasses
 * division entirely by using Euclidean-like steps (similar to GCD) to eliminate
 * row elements.
 *
 * Complexity: O(N^3 log M)
 */
long long det_composite(vector<vector<long long>> a, long long mod)
{
    int n = a.size();
    long long det = 1;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            // Use Euclidean-like steps to eliminate a[j][i] without using fractions
            while (a[j][i] != 0)
            {
                long long t = a[i][i] / a[j][i];

                for (int k = i; k < n; k++)
                {
                    // Subtract a multiple of row j from row i
                    a[i][k] = (a[i][k] - t * a[j][k]) % mod;

                    // Swap row i and row j to keep the algorithm progressing
                    swap(a[i][k], a[j][k]);
                }
                // Every time we swap two rows, the sign of the determinant flips
                det = -det;
            }
        }

        // If a diagonal element becomes 0, the matrix is singular (determinant is 0)
        if (a[i][i] == 0)
            return 0;

        // Multiply the diagonal elements to build the final determinant
        det = det * a[i][i] % mod;
    }

    // Ensure the final determinant is positive before returning
    return (det % mod + mod) % mod;
}