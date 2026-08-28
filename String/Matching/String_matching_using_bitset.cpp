/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: BITSET STRING MATCHING
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Wildcards in text", "Small alphabet", "Multiple patterns".
 * - Classic Scenarios: Text has wildcards. Find all occurrences of pattern.
 * - The Magic: Represents occurrences of each character as a bitset. By shifting
 *   and ANDing the bitsets corresponding to the pattern's characters, you instantly
 *   identify valid shift offsets in O(N * M / 64) time.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       bitset<MAX_N> matches = bitset_match(text, pattern);
 *       // If matches[i] is 1, the pattern exists starting at text[i].
 *
 * 3. ⚙️ HOW TO ADAPT IT
 * - MAX_N must be a constant (e.g., 100005) because bitsets require compile-time sizes.
 */

const int MAX_N = 100005;

bitset<MAX_N> bitset_match(const string &text, const string &pattern)
{
    vector<bitset<MAX_N>> mask(26);
    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] != '?')
        {
            mask[text[i] - 'a'].set(i);
        }
        else
        {
            for (int c = 0; c < 26; c++)
                mask[c].set(i);
        }
    }

    bitset<MAX_N> ans;
    ans.set();

    for (int i = 0; i < pattern.length(); i++)
    {
        if (pattern[i] != '?')
        {
            ans &= (mask[pattern[i] - 'a'] >> i);
        }
    }
    return ans;
}