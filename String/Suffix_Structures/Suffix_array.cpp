struct SuffixArray
{
    string s;
    int n;
    vector<int> p;
    vector<vector<int>> C;
    vector<int> lcp;
    bool is_cyclic;

    SuffixArray(string str, bool cyclic = false)
    {
        is_cyclic = cyclic;
        s = str;
        if (!is_cyclic)
            s += '$';
        n = s.size();
        const int alphapet = 256;
        p.assign(n, 0);
        vector<int> c(n), cnt(max(n, alphapet), 0);

        for (int i = 0; i < n; i++)
            cnt[s[i]]++;
        for (int i = 1; i < alphapet; i++)
            cnt[i] += cnt[i - 1];
        for (int i = 0; i < n; i++)
            p[--cnt[s[i]]] = i;

        c[p[0]] = 0;
        int classes = 1;
        for (int i = 1; i < n; i++)
        {
            if (s[p[i]] != s[p[i - 1]])
                classes++;
            c[p[i]] = classes - 1;
        }
        C.push_back(c);

        vector<int> pn(n), cn(n);
        for (int h = 0; (1 << h) < n; h++)
        {
            for (int i = 0; i < n; i++)
            {
                pn[i] = p[i] - (1 << h);
                if (pn[i] < 0)
                    pn[i] += n;
            }
            fill(cnt.begin(), cnt.begin() + classes, 0ll);
            for (int i = 0; i < n; i++)
                cnt[C.back()[pn[i]]]++;
            for (int i = 1; i < classes; i++)
                cnt[i] += cnt[i - 1];
            for (int i = n - 1; i >= 0; i--)
                p[--cnt[C.back()[pn[i]]]] = pn[i];

            cn[p[0]] = 0;
            classes = 1;
            for (int i = 1; i < n; i++)
            {
                pair<int, int> curr = {C.back()[p[i]], C.back()[(p[i] + (1 << h)) % n]};
                pair<int, int> prev = {C.back()[p[i - 1]], C.back()[(p[i - 1] + (1 << h)) % n]};
                if (curr != prev)
                    classes++;
                cn[p[i]] = classes - 1;
            }
            C.push_back(cn);
        }

        if (!is_cyclic)
        {
            p.erase(p.begin());
            int orig_n = n - 1;
            vector<int> rank(orig_n, 0);
            for (int i = 0; i < orig_n; i++)
                rank[p[i]] = i;
            int k = 0;
            lcp.assign(orig_n - 1, 0);
            for (int i = 0; i < orig_n; i++)
            {
                if (rank[i] == orig_n - 1)
                {
                    k = 0;
                    continue;
                }
                int j = p[rank[i] + 1];
                while (i + k < orig_n && j + k < orig_n && s[i + k] == s[j + k])
                    k++;
                lcp[rank[i]] = k;
                if (k)
                    k--;
            }
        }
    }

    string smallestCyclicShift()
    {
        return s.substr(p[0]) + s.substr(0, p[0]);
    }

    vector<int> subStringInString(string t)
    {
        int m = t.size();
        int orig_n = n - 1;
        int l = 0, r = orig_n - 1;
        int first = -1;
        while (l <= r)
        {
            int mid = l + (r - l) / 2;
            auto can = [&]()
            {
                for (int i = 0; i < m; i++)
                {
                    if (p[mid] + i >= orig_n)
                        return 1LL;
                    if (s[p[mid] + i] != t[i])
                    {
                        if (s[p[mid] + i] < t[i])
                            return 1LL;
                        if (s[p[mid] + i] > t[i])
                            return 2LL;
                    }
                }
                return 0LL;
            };
            int res = can();
            if (res == 0)
            {
                first = mid;
                r = mid - 1;
            }
            else if (res == 1)
            {
                l = mid + 1;
            }
            else
            {
                r = mid - 1;
            }
        }
        if (first == -1)
            return {};
        l = 0, r = orig_n - 1;
        int last = -1;
        while (l <= r)
        {
            int mid = l + (r - l) / 2;
            auto can = [&]()
            {
                for (int i = 0; i < m; i++)
                {
                    if (p[mid] + i >= orig_n)
                        return 1LL;
                    if (s[p[mid] + i] != t[i])
                    {
                        if (s[p[mid] + i] < t[i])
                            return 1LL;
                        if (s[p[mid] + i] > t[i])
                            return 2LL;
                    }
                }
                return 0LL;
            };
            int res = can();
            if (res == 0)
            {
                last = mid;
                l = mid + 1;
            }
            else if (res == 1)
            {
                l = mid + 1;
            }
            else
            {
                r = mid - 1;
            }
        }
        vector<int> ans;
        for (int i = first; i <= last; i++)
        {
            ans.push_back(p[i]);
        }
        sort(ans.begin(), ans.end());
        return ans;
    }

    int compareSubstrings(int i, int len1, int j, int len2)
    {
        int l = min(len1, len2);
        if (l > 0)
        {
            int k = __lg(l);
            pair<int, int> a = {C[k][i], C[k][i + l - (1 << k)]};
            pair<int, int> b = {C[k][j], C[k][j + l - (1 << k)]};
            if (a != b)
                return a < b ? -1 : 1;
        }
        if (len1 == len2)
            return 0;
        return len1 < len2 ? -1 : 1;
    }

    int numberOfDifferentSubStrings()
    {
        int orig_n = n - 1;
        return (orig_n * orig_n + orig_n) / 2 - accumulate(lcp.begin(), lcp.end(), 0ll);
    }

    string longestCommonSubstring(string t)
    {
        string orig_s = s;
        if (!is_cyclic)
            orig_s.pop_back();
        int len1 = orig_s.size();
        SuffixArray sa(orig_s + char(1) + t);
        int max_len = 0;
        int start_idx = -1;
        for (int i = 0; i < (int)sa.lcp.size(); i++)
        {
            if ((sa.p[i] < len1 && sa.p[i + 1] > len1) || (sa.p[i] > len1 && sa.p[i + 1] < len1))
            {
                if (sa.lcp[i] > max_len)
                {
                    max_len = sa.lcp[i];
                    start_idx = sa.p[i];
                }
            }
        }
        if (max_len == 0)
            return "";
        return sa.s.substr(start_idx, max_len);
    }

    string longestRepeatedSubstring()
    {
        int max_len = 0;
        int start_idx = -1;
        for (int i = 0; i < (int)lcp.size(); i++)
        {
            if (lcp[i] > max_len)
            {
                max_len = lcp[i];
                start_idx = p[i];
            }
        }
        if (max_len == 0)
            return "";
        return s.substr(start_idx, max_len);
    }
};
