struct Node
{
    vector<int> v;

    Node()
    {
    }

    Node(int val)
    {
        v.push_back(val);
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
        ans.v.resize(int(lf.v.size()) + int(rt.v.size()));
        std::merge(lf.v.begin(), lf.v.end(), rt.v.begin(), rt.v.end(), ans.v.begin());
        return ans;
    }

    int get(int l, int r, int k, int node, int lx, int rx)
    {
        if (lx >= l && rx <= r)
            return distance(upper_bound(seg[node].v.begin(), seg[node].v.end(), k), seg[node].v.end());
        if (rx <= l || lx >= r)
            return 0;
        int mid = (lx + rx) / 2;
        int lf = get(l, r, k, 2 * node + 1, lx, mid);
        int ri = get(l, r, k, 2 * node + 2, mid, rx);
        return lf + ri;
    }

    int get(int l, int r, int k)
    {
        return get(l, r, k, 0, 0, sz);
    }
};
