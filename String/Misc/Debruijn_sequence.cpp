/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: DE BRUIJN SEQUENCE
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Contains all permutations", "Shortest string with all substrings".
 * - Classic Scenarios: Generate the shortest possible string that contains every
 *   possible sequence of length N using an alphabet of size K.
 * - The Magic: Mathematical guarantees state this string will always have length
 *   K^N + N - 1. This uses a recursive DFS (essentially finding an Eulerian path)
 *   to generate the lexicographically smallest De Bruijn sequence in optimal time.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       string sequence = de_bruijn(K, N);
 * - Example:
 *       de_bruijn(2, 3) generates "00010111".
 *       (Note: To act as a true cyclic sequence, you conceptually wrap the ends.
 *       If you need it strictly linear, append the first N-1 characters to the end).
 *
 * - Complexity:
 *       Time: O(K^N)
 *       Space: O(K * N)
 */

string de_bruijn(int k, int n)
{
    if (k == 1)
        return string(n, '0');
    if (n == 1)
    {
        string res = "";
        for (int i = 0; i < k; i++)
            res += (char)('0' + i);
        return res;
    }

    vector<int> a(k * n, 0);
    string sequence;

    auto db = [&](auto &self, int t, int p) -> void
    {
        if (t > n)
        {
            if (n % p == 0)
            {
                for (int i = 1; i <= p; i++)
                {
                    sequence += (char)('0' + a[i]);
                }
            }
        }
        else
        {
            a[t] = a[t - p];
            self(self, t + 1, p);
            for (int j = a[t - p] + 1; j < k; j++)
            {
                a[t] = j;
                self(self, t + 1, t);
            }
        }
    };

    db(db, 1, 1);
    return sequence;
}