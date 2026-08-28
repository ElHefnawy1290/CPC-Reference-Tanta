struct MultisetHashing
{
    int base, m, n;
    vector<int> pre;

    int power(int b, int p)
    {
        b %= m;
        int res{1};
        while (p)
        {
            if (p & 1)
                res = res * b % m;
            b = b * b % m;
            p >>= 1;
        }
        return res;
    }

    MultisetHashing(vector<int> &arr, int b)
    {
        n = int(arr.size());
        m = 1e9 + 7;
        base = b;
        pre.assign(n + 1, {});
        for (int i = 0; i < n; i++)
            pre[i + 1] = (pre[i] + power(base, arr[i])) % m;
    }

    int get(int l, int r)
    {
        // 0-based
        return (pre[r + 1] - pre[l] + m) % m;
    }
};
