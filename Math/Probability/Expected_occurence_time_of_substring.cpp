/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: EXPECTED OCCURRENCE TIME OF SUBSTRING
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Expected number of coin flips", "Typing monkeys", "Expected time to see string".
 * - Classic Scenarios: A random generator picks characters from an alphabet of size A.
 *   What is the expected number of generated characters until string S appears?
 * - The Magic: Bypasses O(N^3) Gaussian elimination. By setting up a fair casino
 *   Martingale where gamblers bet on consecutive characters, the expected time simplifies
 *   to a summation over the borders (prefix == suffix) of the string.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       long long expected_time = expected_string_occurrence(S, ALPHABET_SIZE, MOD);
 *
 * - Complexity:
 *       Time: O(|S|)
 *       Space: O(|S|)
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Alphabet Size: If flipping a coin, alphabet_size = 2. If rolling a die, 6.
 * - Array Form: This strictly uses the KMP prefix function. The condition `j == pi[j-1]`
 *   efficiently finds all prefix-suffix borders of the entire string.
 */

using ll = long long;

vector<int> compute_pi(const string &s)
{
    int n = s.length();
    vector<int> pi(n);
    for (int i = 1; i < n; i++)
    {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j])
            j = pi[j - 1];
        if (s[i] == s[j])
            j++;
        pi[i] = j;
    }
    return pi;
}

ll expected_string_occurrence(const string &s, ll alphabet_size, ll mod)
{
    vector<int> pi = compute_pi(s);
    int n = s.length();

    auto binpow = [&](ll base, ll exp)
    {
        ll res = 1;
        base %= mod;
        while (exp > 0)
        {
            if (exp % 2 == 1)
                res = (res * base) % mod;
            base = (base * base) % mod;
            exp /= 2;
        }
        return res;
    };

    ll expected_time = 0;
    int current_border = n;

    while (current_border > 0)
    {
        expected_time = (expected_time + binpow(alphabet_size, current_border)) % mod;
        current_border = pi[current_border - 1];
    }

    return expected_time;
}