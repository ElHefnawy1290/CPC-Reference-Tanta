struct Node
{
    int val, lazy;
    bool is_lazy;

    Node()
    {
        val = 0, lazy = 0, is_lazy = false;
    }

    Node(int val) : val(val)
    {
        lazy = 0, is_lazy = false;
    }

    void change(int x, int lx, int rx)
    {
        val += x * (rx - lx);
        lazy += x;
        is_lazy = true;
    }

    void reset()
    {
        lazy = 0, is_lazy = false;
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
        ans.val = lf.val + rt.val;
        return ans;
    }

    void push(int node, int lx, int rx)
    {
        if (rx - lx == 1 || !seg[node].is_lazy)
            return;
        int mid = (lx + rx) / 2;
        seg[2 * node + 1].change(seg[node].lazy, lx, mid);
        seg[2 * node + 2].change(seg[node].lazy, mid, rx);
        seg[node].reset();
    }

    void update(int l, int r, int x, int node, int lx, int rx)
    {
        if (lx >= r || rx <= l)
            return;
        push(node, lx, rx);
        if (lx >= l && rx <= r)
        {
            seg[node].change(x, lx, rx);
            return;
        }
        int mid = (lx + rx) / 2;
        update(l, r, x, 2 * node + 1, lx, mid);
        update(l, r, x, 2 * node + 2, mid, rx);

        seg[node] = merge(seg[2 * node + 1], seg[2 * node + 2]);
    }

    void update(int l, int r, int x)
    {
        return update(l, r, x, 0, 0, sz);
    }

    Node get(int l, int r, int node, int lx, int rx)
    {
        if (rx <= l || lx >= r)
            return Node();
        push(node, lx, rx);
        if (lx >= l && rx <= r)
            return seg[node];
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
