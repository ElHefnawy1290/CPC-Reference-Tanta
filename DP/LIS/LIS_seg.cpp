struct Node
{
    int val;

    Node()
    {
        val = 0;
    }

    Node(int val)
        : val(val)
    {
    }

    void change(int x)
    {
        val = max(x, val);
    }
};

struct segmentTree
{
    int sz;
    vector<Node> seg;

    segmentTree(int n)
    {
        sz = 1;
        while (sz < n)
            sz *= 2;
        seg.assign(2 * sz, Node());
    }

    void init(vector<int> &arr, int node, int lx, int rx)
    {
        if (rx - lx == 1)
        {
            if (lx < int(arr.size()))
                seg[node] = Node(arr[lx]);
            return;
        }
        int mid = (lx + rx) / 2;
        init(arr, 2 * node + 1, lx, mid);
        init(arr, 2 * node + 2, mid, rx);
        seg[node] = merge(seg[2 * node + 1], seg[2 * node + 2]);
    }

    void init(vector<int> &arr)
    {
        init(arr, 0, 0, sz);
    }

    Node merge(Node &lf, Node &rt)
    {
        Node ans = Node();
        ans.val = max(lf.val, rt.val);
        return ans;
    }

    void set(int idx, int val, int node, int lx, int rx)
    {
        if (rx - lx == 1)
        {
            seg[node].change(val);
            return;
        }
        int mid = (lx + rx) / 2;
        if (idx < mid)
            set(idx, val, 2 * node + 1, lx, mid);
        else
            set(idx, val, 2 * node + 2, mid, rx);
        seg[node] = merge(seg[2 * node + 1], seg[2 * node + 2]);
    }

    void set(int idx, int val)
    {
        set(idx, val, 0, 0, sz);
    }

    Node get(int l, int r, int node, int lx, int rx)
    {
        if (lx >= l && rx <= r)
            return seg[node];
        if (rx <= l || lx >= r)
            return Node();
        int mid = (lx + rx) / 2;
        Node lf = get(l, r, 2 * node + 1, lx, mid);
        Node ri = get(l, r, 2 * node + 2, mid, rx);
        return merge(lf, ri);
    }

    int get(int l, int r)
    {
        return get(l, r, 0, 0, sz).val;
    }
};

int LIS(int n, vector<int> &p)
{
    segmentTree seg(n + 5);
    vector<int> dp(n + 1);
    int ans{};
    for (int i = 0; i < n; i++)
    {
        dp[i] = seg.get(0, p[i]) + 1;
        seg.set(p[i], dp[i]);
        ans = max(ans, dp[i]);
    }
    return ans;
}
