/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: AHO-CORASICK
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Multiple patterns", "Dictionary matching", "String DP".
 * - Classic Scenarios: Find all occurrences of an array of strings inside a massive text,
 *   or count how many strings of length N don't contain any of the dictionary words.
 * - The Magic: Builds a Trie and connects nodes via "fail" links. If you mismatch,
 *   the fail link teleports you to the longest proper suffix that exists in the Trie.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization:
 *       AhoCorasick ac;
 *       for (string s : patterns) ac.insert(s);
 *       ac.build();
 * - Query:
 *       int total_matches = ac.search(text);
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - DP Transitions: The `next_state(u, c)` transitions are precalculated during `build()`.
 *   You can use `ac.trie[u].next[c]` to transition seamlessly in Dynamic Programming.
 * - Exit Link (dict_link): Fast-forwards to the next valid dictionary word ending to
 *   avoid O(N) worst-case walking up the failure tree.
 */

const int K = 26;

struct Vertex
{
    int next[K];
    bool leaf = false;
    int p = -1;
    char pch;
    int link = -1;
    int exit_link = -1;
    int matches = 0; // Count of dictionary words ending here

    Vertex(int p = -1, char ch = '$') : p(p), pch(ch)
    {
        fill(begin(next), end(next), -1);
    }
};

struct AhoCorasick
{
    vector<Vertex> t;

    AhoCorasick()
    {
        t.emplace_back();
    }

    void insert(const string &s)
    {
        int v = 0;
        for (char ch : s)
        {
            int c = ch - 'a';
            if (t[v].next[c] == -1)
            {
                t[v].next[c] = t.size();
                t.emplace_back(v, ch);
            }
            v = t[v].next[c];
        }
        t[v].leaf = true;
        t[v].matches++;
    }

    void build()
    {
        queue<int> q;
        q.push(0);
        t[0].link = 0;
        t[0].exit_link = 0;

        while (!q.empty())
        {
            int v = q.front();
            q.pop();

            for (int c = 0; c < K; c++)
            {
                int u = t[v].next[c];
                if (u != -1)
                {
                    t[u].link = (v == 0) ? 0 : t[t[v].link].next[c];
                    t[u].exit_link = t[t[u].link].leaf ? t[u].link : t[t[u].link].exit_link;
                    t[u].matches += t[t[u].link].matches;
                    q.push(u);
                }
                else
                {
                    t[v].next[c] = (v == 0) ? 0 : t[t[v].link].next[c];
                }
            }
        }
    }

    long long search(const string &text)
    {
        long long total = 0;
        int v = 0;
        for (char ch : text)
        {
            v = t[v].next[ch - 'a'];
            total += t[v].matches;
        }
        return total;
    }
};