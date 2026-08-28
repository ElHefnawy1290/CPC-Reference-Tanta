struct ZAlgorithm
{
    vector<int> build(const string &s)
    {
        int n = s.size();
        vector<int> z(n, 0);
        int l = 0, r = 0;
        for (int i = 1; i < n; i++)
        {
            if (i < r)
                z[i] = min(r - i, z[i - l]);
            while (i + z[i] < n && s[z[i]] == s[i + z[i]])
                z[i]++;
            if (i + z[i] > r)
            {
                l = i;
                r = i + z[i];
            }
        }
        return z;
    }

    int numberOfMatches(const string &s, const string &p)
    {
        if (p.empty() || s.empty())
            return 0;
        string combined = p + "$" + s;
        vector<int> z = build(combined);
        int ans = 0;
        int p_len = p.size();
        for (int i = p_len + 1; i < z.size(); i++)
        {
            if (z[i] == p_len)
                ans++;
        }
        return ans;
    }

    vector<int> findOccurrences(const string &s, const string &p)
    {
        if (p.empty() || s.empty())
            return {};
        string combined = p + "$" + s;
        vector<int> z = build(combined);
        vector<int> occ;
        int p_len = p.size();
        for (int i = p_len + 1; i < z.size(); i++)
        {
            if (z[i] == p_len)
                occ.push_back(i - p_len - 1);
        }
        return occ;
    }
};
