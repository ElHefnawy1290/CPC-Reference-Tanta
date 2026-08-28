struct Manacher
{
    vector<int> p;
    string s;

    Manacher(const string &str)
    {
        s = str;
        string t = "";
        for (char c : s)
        {
            t += "#";
            t += c;
        }
        t += "#";

        int n = t.size();
        string temp = "$" + t + "^";
        vector<int> p_temp(n + 2, 0);
        int l = 0, r = 1;

        for (int i = 1; i <= n; i++)
        {
            if (i <= r)
                p_temp[i] = min(r - i, p_temp[l + (r - i)]);
            while (temp[i - p_temp[i]] == temp[i + p_temp[i]])
                p_temp[i]++;
            if (i + p_temp[i] > r)
            {
                l = i - p_temp[i];
                r = i + p_temp[i];
            }
        }

        p.assign(n, 0);
        for (int i = 1; i <= n; i++)
        {
            p[i - 1] = p_temp[i] - 1;
        }
    }

    bool is_palindrome(int l, int r)
    {
        if (l > r)
            swap(l, r);
        int len = r - l + 1;
        int idx = l + r + 1;
        return p[idx] >= len;
    }

    string longest_palindrome()
    {
        int max_len = 0, center_idx = 0;
        for (int i = 0; i < p.size(); i++)
        {
            if (p[i] > max_len)
            {
                max_len = p[i];
                center_idx = i;
            }
        }
        int start = (center_idx - max_len) / 2;
        return s.substr(start, max_len);
    }
};
