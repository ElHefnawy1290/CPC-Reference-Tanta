vector<int> KMP(string s, string p = "")
{
    if (!p.empty())
        s = p + "$" + s;
    int n = int(s.size());
    vector<int> LPS(n);
    int i{1}, j{};
    while (i < n)
    {
        if (s[i] == s[j])
        {
            j++;
            LPS[i] = j;
            i++;
        }
        else
        {
            if (j == 0)
            {
                LPS[i] = 0;
                i++;
            }
            else
                j = LPS[j - 1];
        }
    }
    return LPS;
}

int numberOfMatches(string &s, string &p)
{
    vector<int> pi = KMP(s, p);
    int ans{};
    for (auto &val : pi)
        ans += val == int(p.size());
    return ans;
}

vector<int> countPrefixMatching(string &s)
{
    int n = int(s.size());
    vector<int> pi = KMP(s);
    vector<int> frq(n);
    for (auto &x : pi)
        frq[x]++;
    for (int i = n - 1; i; i--)
        frq[pi[i - 1]] += frq[i];
    return frq;
}

vector<vector<int>> KMPAutomaton(string &s)
{
    vector<int> pi = KMP(s);
    int n = int(s.size());
    vector aut(n + 1, vector<int>(26));
    aut[0][s[0] - 'a'] = 1;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            if (i < n && s[i] - 'a' == j)
                aut[i][j] = i + 1;
            else
                aut[i][j] = aut[pi[i - 1]][j];
        }
    }
    return aut;
}
