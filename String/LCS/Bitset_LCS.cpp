/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: BITSET LCS
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "LCS of massive strings", "O(N^2) TLE".
 * - Classic Scenarios: Find the Longest Common Subsequence of two strings where
 *   N and M are up to 50,000.
 * - The Magic: C++ `std::bitset` does not support subtraction, which is required
 *   for the classic bitwise LCS formula. This custom block engine maps the DP states
 *   directly onto native 64-bit unsigned integers (uint64_t). It computes 64 DP
 *   cells simultaneously using carry-flag subtraction, slashing the O(N * M) time
 *   by a factor of 64.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       int length = bitset_lcs(string_A, string_B);
 *
 * - Complexity:
 *       Time: O(N * M / 64)
 *       Space: O(M / 64)
 */

#include <cstdint>

int bitset_lcs(const string &a, const string &b)
{
    int n = a.length(), m = b.length();
    if (n == 0 || m == 0)
        return 0;

    int words = (m >> 6) + 1;
    vector<vector<uint64_t>> pos(256, vector<uint64_t>(words, 0));

    for (int i = 0; i < m; i++)
    {
        pos[b[i]][i >> 6] |= (1ULL << (i & 63));
    }

    vector<uint64_t> D(words, 0);

    for (int i = 0; i < n; i++)
    {
        uint64_t b_carry = 1;
        uint64_t shift_carry = 0;

        for (int j = 0; j < words; j++)
        {
            uint64_t P = D[j] | pos[a[i]][j];
            uint64_t d_shl = (D[j] << 1) | shift_carry;
            shift_carry = D[j] >> 63;

            uint64_t sub = d_shl + b_carry;
            uint64_t next_carry = (P < sub) || (sub < d_shl);
            uint64_t diff = P - sub;

            D[j] = P & (~P ^ diff);
            b_carry = next_carry;
        }
    }

    int lcs_length = 0;
    for (int j = 0; j < words; j++)
    {
        uint64_t val = D[j];
        while (val)
        {
            val &= (val - 1);
            lcs_length++;
        }
    }

    return lcs_length;
}