struct Hash
{
    int base, mod1, mod2, inv1, inv2, n;
    vector<int> pw1, pw2, invpw1, invpw2, pre1, pre2;

    int power(int b, int p, int m)
    {
        int res{1};
        b %= m;
        while (p)
        {
            if (p & 1)
                res = res * b % m;
            b = b * b % m;
            p >>= 1;
        }
        return res;
    }

    Hash(const string &s, int b)
    {
        base = b;
        mod1 = 1e9 + 7;
        mod2 = 1e9 + 9;
        n = int(s.size());

        inv1 = power(base, mod1 - 2, mod1);
        inv2 = power(base, mod2 - 2, mod2);

        pw1.assign(n + 1, 1), pw2.assign(n + 1, 1);
        invpw1.assign(n + 1, 1), invpw2.assign(n + 1, 1);
        pre1.assign(n + 1, {}), pre2.assign(n + 1, {});

        for (int i = 0; i < n; i++)
        {
            pw1[i + 1] = pw1[i] * base % mod1;
            pw2[i + 1] = pw2[i] * base % mod2;
            invpw1[i + 1] = invpw1[i] * inv1 % mod1;
            invpw2[i + 1] = invpw2[i] * inv2 % mod2;
            pre1[i + 1] = (pre1[i] + pw1[i] * s[i]) % mod1;
            pre2[i + 1] = (pre2[i] + pw2[i] * s[i]) % mod2;
        }
    }

    pair<int, int> get(int l, int r)
    {
        return {(pre1[r + 1] - pre1[l] + mod1) * invpw1[l] % mod1, (pre2[r + 1] - pre2[l] + mod2) * invpw2[l] % mod2};
    }
};
