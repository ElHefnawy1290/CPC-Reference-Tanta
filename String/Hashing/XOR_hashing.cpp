struct XorHashing
{
    int n;
    map<int, int> h;
    vector<int> pre;

    XorHashing(vector<int> &arr)
    {
        n = int(arr.size());
        pre.assign(n + 1, {});
        for (int i = 0; i < n; i++)
        {
            if (!h.count(arr[i]))
                h[arr[i]] = getRandom(1, 1e18);
            pre[i + 1] = pre[i] ^ h[arr[i]];
        }
    }

    int get(int l, int r)
    {
        // 0-based
        return pre[r + 1] ^ pre[l];
    }
};
