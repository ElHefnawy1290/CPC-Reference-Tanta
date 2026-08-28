/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: FFT WILDCARD MATCHING
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Wildcards in both text and pattern", "Massive strings".
 * - Classic Scenarios: Both strings have '?' wildcards. Bitsets will TLE if
 *   N = 200,000.
 * - The Magic: Uses the algebraic identity (T - P)^2 * T * P = 0. Maps wildcards
 *   to 0 and letters to 1-26. Expands the polynomial into three separate NTT
 *   convolutions. A result of 0 at index i means a perfect match.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       vector<int> matches = fft_wildcard_match(text, pattern);
 *
 * 3. ⚙️ HOW TO ADAPT IT
 * - Dependencies: This heavily relies on your `multiply(A, B)` function from
 *   your NTT template. Make sure it is included above this.
 */

using ll = long long;

vector<ll> multiply(vector<ll> const &a, vector<ll> const &b);

vector<int> fft_wildcard_match(const string &text, const string &pattern)
{
    int n = text.length(), m = pattern.length();
    if (m > n)
        return {};

    vector<ll> t1(n), t2(n), t3(n);
    vector<ll> p1(m), p2(m), p3(m);

    for (int i = 0; i < n; i++)
    {
        ll val = (text[i] == '?' ? 0 : text[i] - 'a' + 1);
        t1[i] = val;
        t2[i] = val * val;
        t3[i] = val * val * val;
    }

    for (int i = 0; i < m; i++)
    {
        ll val = (pattern[i] == '?' ? 0 : pattern[i] - 'a' + 1);
        p1[m - 1 - i] = val;
        p2[m - 1 - i] = val * val;
        p3[m - 1 - i] = val * val * val;
    }

    vector<ll> res1 = multiply(t3, p1);
    vector<ll> res2 = multiply(t2, p2);
    vector<ll> res3 = multiply(t1, p3);

    vector<int> matches;
    for (int i = m - 1; i < n; i++)
    {
        ll diff = res1[i] - 2 * res2[i] + res3[i];
        if (diff == 0)
        {
            matches.push_back(i - (m - 1));
        }
    }
    return matches;
}