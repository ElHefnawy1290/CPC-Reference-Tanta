struct SuffixAutomaton
{
    struct State
    {
        int len, link;
        int first_pos;
        bool is_clone;
        int cnt;
        long long paths;
        map<char, int> next;
        vector<int> inv_link;
    };

    vector<State> st;
    int sz, last;
    string s;

    SuffixAutomaton()
    {
        st.assign(2, State());
        st[0].len = 0;
        st[0].link = -1;
        st[0].cnt = 0;
        sz = 1;
        last = 0;
    }

    void extend(char c)
    {
        int cur = sz++;
        if (cur >= st.size())
            st.resize(st.size() * 2);
        st[cur].len = st[last].len + 1;
        st[cur].first_pos = st[cur].len - 1;
        st[cur].is_clone = false;
        st[cur].cnt = 1;

        s += c;

        int p = last;
        while (p != -1 && !st[p].next.count(c))
        {
            st[p].next[c] = cur;
            p = st[p].link;
        }
        if (p == -1)
        {
            st[cur].link = 0;
        }
        else
        {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len)
            {
                st[cur].link = q;
            }
            else
            {
                int clone = sz++;
                if (clone >= st.size())
                    st.resize(st.size() * 2);
                st[clone].len = st[p].len + 1;
                st[clone].next = st[q].next;
                st[clone].link = st[q].link;
                st[clone].first_pos = st[q].first_pos;
                st[clone].is_clone = true;
                st[clone].cnt = 0;

                while (p != -1 && st[p].next[c] == q)
                {
                    st[p].next[c] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }

    void build(const string &text)
    {
        for (char c : text)
            extend(c);
        precompute();
    }

    void precompute()
    {
        for (int v = 1; v < sz; v++)
        {
            st[st[v].link].inv_link.push_back(v);
        }
        vector<pair<int, int>> order(sz);
        for (int i = 0; i < sz; i++)
            order[i] = {st[i].len, i};
        sort(order.rbegin(), order.rend());
        for (int i = 0; i < sz - 1; i++)
        {
            int v = order[i].second;
            if (st[v].link != -1)
            {
                st[st[v].link].cnt += st[v].cnt;
            }
        }
    }

    bool checkOccurrence(string P)
    {
        int v = 0;
        for (char c : P)
        {
            if (!st[v].next.count(c))
                return false;
            v = st[v].next[c];
        }
        return true;
    }

    long long numberOfDifferentSubstrings()
    {
        long long tot = 0;
        for (int i = 1; i < sz; i++)
        {
            tot += st[i].len - st[st[i].link].len;
        }
        return tot;
    }

    long long totalLengthOfDifferentSubstrings()
    {
        long long tot = 0;
        for (int i = 1; i < sz; i++)
        {
            long long shortest = st[st[i].link].len + 1;
            long long longest = st[i].len;
            long long num_strings = longest - shortest + 1;
            long long cur = num_strings * (longest + shortest) / 2;
            tot += cur;
        }
        return tot;
    }

    void precomputePaths()
    {
        vector<pair<int, int>> order(sz);
        for (int i = 0; i < sz; i++)
            order[i] = {st[i].len, i};
        sort(order.rbegin(), order.rend());
        for (int i = 0; i < sz; i++)
        {
            int v = order[i].second;
            st[v].paths = st[v].cnt;
            for (auto &edge : st[v].next)
            {
                st[v].paths += st[edge.second].paths;
            }
        }
    }

    string kthSubstring(long long k)
    {
        precomputePaths();
        int v = 0;
        string ans = "";
        while (k > 0)
        {
            for (auto &edge : st[v].next)
            {
                int w = edge.second;
                if (st[w].paths >= k)
                {
                    ans += edge.first;
                    k -= st[w].cnt;
                    v = w;
                    break;
                }
                else
                {
                    k -= st[w].paths;
                }
            }
        }
        return ans;
    }

    static string smallestCyclicShift(string S)
    {
        SuffixAutomaton sa;
        sa.build(S + S);
        int v = 0;
        string ans = "";
        for (int i = 0; i < S.size(); i++)
        {
            auto edge = *sa.st[v].next.begin();
            ans += edge.first;
            v = edge.second;
        }
        return ans;
    }

    int numberOfOccurrences(string P)
    {
        int v = 0;
        for (char c : P)
        {
            if (!st[v].next.count(c))
                return 0;
            v = st[v].next[c];
        }
        return st[v].cnt;
    }

    int firstOccurrencePosition(string P)
    {
        int v = 0;
        for (char c : P)
        {
            if (!st[v].next.count(c))
                return -1;
            v = st[v].next[c];
        }
        return st[v].first_pos - P.length() + 1;
    }

    void dfsOccurrences(int v, int P_length, vector<int> &res)
    {
        if (!st[v].is_clone)
        {
            res.push_back(st[v].first_pos - P_length + 1);
        }
        for (int u : st[v].inv_link)
        {
            dfsOccurrences(u, P_length, res);
        }
    }

    vector<int> allOccurrencePositions(string P)
    {
        int v = 0;
        for (char c : P)
        {
            if (!st[v].next.count(c))
                return {};
            v = st[v].next[c];
        }
        vector<int> res;
        dfsOccurrences(v, P.length(), res);
        sort(res.begin(), res.end());
        return res;
    }

    string shortestNonAppearingString(char min_char = 'a', char max_char = 'z')
    {
        vector<int> d(sz, -1);
        auto dfs = [&](auto &self, int v) -> int
        {
            if (d[v] != -1)
                return d[v];
            d[v] = 1e9;
            for (char c = min_char; c <= max_char; c++)
            {
                if (!st[v].next.count(c))
                {
                    d[v] = 1;
                    break;
                }
                else
                {
                    d[v] = min(d[v], 1 + self(self, st[v].next[c]));
                }
            }
            return d[v];
        };
        dfs(dfs, 0);
        string ans = "";
        int v = 0;
        while (true)
        {
            for (char c = min_char; c <= max_char; c++)
            {
                if (!st[v].next.count(c))
                {
                    ans += c;
                    return ans;
                }
            }
            for (char c = min_char; c <= max_char; c++)
            {
                if (d[v] == 1 + d[st[v].next[c]])
                {
                    ans += c;
                    v = st[v].next[c];
                    break;
                }
            }
        }
    }

    string longestCommonSubstringTwo(string T)
    {
        int v = 0, l = 0, best = 0, bestpos = 0;
        for (int i = 0; i < T.size(); i++)
        {
            while (v && !st[v].next.count(T[i]))
            {
                v = st[v].link;
                l = st[v].len;
            }
            if (st[v].next.count(T[i]))
            {
                v = st[v].next[T[i]];
                l++;
            }
            if (l > best)
            {
                best = l;
                bestpos = i;
            }
        }
        return T.substr(bestpos - best + 1, best);
    }

    static string longestCommonSubstringMultiple(vector<string> strings)
    {
        if (strings.empty())
            return "";
        if (strings.size() == 1)
            return strings[0];

        int min_idx = 0;
        for (int i = 1; i < strings.size(); i++)
        {
            if (strings[i].size() < strings[min_idx].size())
            {
                min_idx = i;
            }
        }
        swap(strings[0], strings[min_idx]);

        SuffixAutomaton sa;
        sa.build(strings[0]);

        vector<int> order(sa.sz);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b)
             { return sa.st[a].len > sa.st[b].len; });

        vector<int> min_max_l(sa.sz);
        for (int i = 0; i < sa.sz; i++)
        {
            min_max_l[i] = sa.st[i].len;
        }

        for (int i = 1; i < strings.size(); i++)
        {
            vector<int> max_l(sa.sz, 0);
            int v = 0, l = 0;
            for (char c : strings[i])
            {
                while (v > 0 && !sa.st[v].next.count(c))
                {
                    v = sa.st[v].link;
                    l = sa.st[v].len;
                }
                if (sa.st[v].next.count(c))
                {
                    v = sa.st[v].next[c];
                    l++;
                }
                max_l[v] = max(max_l[v], l);
            }

            for (int u : order)
            {
                min_max_l[u] = min(min_max_l[u], max_l[u]);
                int lnk = sa.st[u].link;
                if (lnk != -1)
                {
                    max_l[lnk] = max(max_l[lnk], min(max_l[u], sa.st[lnk].len));
                }
            }
        }

        int best_len = 0;
        int best_v = 0;
        for (int i = 1; i < sa.sz; i++)
        {
            if (min_max_l[i] > best_len)
            {
                best_len = min_max_l[i];
                best_v = i;
            }
        }
        if (best_len == 0)
            return "";
        return strings[0].substr(sa.st[best_v].first_pos - best_len + 1, best_len);
    }
};
