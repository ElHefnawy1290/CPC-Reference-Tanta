/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: PALINDROMIC TREE (EERTREE)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Distinct palindromes", "Occurrences of palindromes".
 * - Classic Scenarios: Find the number of distinct palindromic substrings, or
 *   find the palindrome P that maximizes length(P) * occurrences(P).
 * - The Magic: It builds a graph containing exactly two roots: one for even-length
 *   palindromes and one for odd-length. Every node represents a distinct palindrome,
 *   and fail links point to the longest proper palindromic suffix.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization:
 *       Eertree pt;
 *       for (char c : S) pt.add_char(c);
 *       pt.count_occurrences(); // Must call this after building!
 *
 * - Complexity:
 *       Time: Amortized O(N)
 *       Space: O(N * ALPHABET_SIZE)
 */

struct Eertree
{
    static const int K = 26;
    struct Node
    {
        int next[K];
        int len;
        int link;
        long long cnt;

        Node(int len = 0, int link = 0) : len(len), link(link), cnt(0)
        {
            fill(begin(next), end(next), 0);
        }
    };

    vector<Node> tree;
    string s;
    int last;

    Eertree()
    {
        tree.emplace_back(0, 1);  // Node 0: Even length root
        tree.emplace_back(-1, 1); // Node 1: Odd length root
        last = 0;
    }

    void add_char(char ch)
    {
        s += ch;
        int c = ch - 'a';
        int curr = last;
        int pos = s.length() - 1;

        while (pos - 1 - tree[curr].len < 0 || s[pos - 1 - tree[curr].len] != ch)
        {
            curr = tree[curr].link;
        }

        if (tree[curr].next[c])
        {
            last = tree[curr].next[c];
            tree[last].cnt++;
            return;
        }

        int now = tree.size();
        tree.emplace_back(tree[curr].len + 2);
        tree[curr].next[c] = now;

        if (tree[now].len == 1)
        {
            tree[now].link = 0;
        }
        else
        {
            int fail = tree[curr].link;
            while (pos - 1 - tree[fail].len < 0 || s[pos - 1 - tree[fail].len] != ch)
            {
                fail = tree[fail].link;
            }
            tree[now].link = tree[fail].next[c];
        }

        last = now;
        tree[last].cnt++;
    }

    void count_occurrences()
    {
        for (int i = tree.size() - 1; i > 1; i--)
        {
            tree[tree[i].link].cnt += tree[i].cnt;
        }
    }
};