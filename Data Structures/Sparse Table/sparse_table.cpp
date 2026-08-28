struct SparseTable
{
    int n, LG;
    vector<vector<int>> T;

    SparseTable(const vector<int> &arr)
    {
        n = arr.size();
        LG = __lg(n) + 2;
        T.assign(LG, vector<int>(n));
        for (int i = 0; i < n; i++)
            T[0][i] = arr[i];
        for (int pw = 1; (1 << pw) <= n; pw++)
        {
            for (int i = 0; i + (1 << pw) <= n; i++)
                T[pw][i] = merge(T[pw - 1][i], T[pw - 1][i + (1 << (pw - 1))]);
        }
    }

    int merge(int a, int b)
    {
        return min(a, b);
    }

    int query(int l, int r)
    {
        int len = r - l + 1;
        int pw = __lg(len);
        return merge(T[pw][l], T[pw][r - (1 << pw) + 1]);
    }
};
