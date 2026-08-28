/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: DUVAL'S & MIN CYCLIC SHIFT
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Lexicographically smallest rotation", "Lyndon words".
 * - Classic Scenarios: You can rotate a string S any number of times. Find the
 *   rotation that comes first in the dictionary.
 * - The Magic: Duval's algorithm processes the string in strict O(N) time and
 *   O(1) auxiliary space, bypassing the need to build a heavy Suffix Automaton
 *   on the concatenated string `S + S`.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       int best_index = min_cyclic_shift(S);
 *       string smallest_rotation = S.substr(best_index) + S.substr(0, best_index);
 *       vector<string> factorization = duval_factorization(S);
 *
 * - Complexity:
 *       Time: O(N)
 *       Space: O(1) for shift, O(N) for factorization return.
 */

int min_cyclic_shift(string s)
{
    s += s;
    int n = s.length();
    int i = 0, ans = 0;
    while (i < n / 2)
    {
        ans = i;
        int j = i + 1, k = i;
        while (j < n && s[k] <= s[j])
        {
            if (s[k] < s[j])
                k = i;
            else
                k++;
            j++;
        }
        while (i <= k)
            i += j - k;
    }
    return ans;
}

vector<string> duval_factorization(const string &s)
{
    int n = s.length();
    int i = 0;
    vector<string> factorization;
    while (i < n)
    {
        int j = i + 1, k = i;
        while (j < n && s[k] <= s[j])
        {
            if (s[k] < s[j])
                k = i;
            else
                k++;
            j++;
        }
        while (i <= k)
        {
            factorization.push_back(s.substr(i, j - k));
            i += j - k;
        }
    }
    return factorization;
}