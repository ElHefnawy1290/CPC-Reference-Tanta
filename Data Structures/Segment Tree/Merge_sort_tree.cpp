struct Node
{
    vector<int> v;
};

struct segTree
{
    vector<Node> seg;
    int sz;

    segTree(int n)
    {
        sz = 1;
        while (sz < n)
            sz *= 2;
        seg.assign(2 * sz, Node());
    }

    Node merge(Node &l, Node &r)
    {
        Node res;
        res.v.reserve(l.v.size() + r.v.size());
        std::merge(all(l.v), all(r.v), back_inserter(res.v));
        return res;
    }

    void build(vector<int> &arr, int ni, int lx, int rx)
    {
        if (rx - lx == 1)
        {
            if (lx < (int)arr.size())
            {
                seg[ni].v = {arr[lx]};
            }
            return;
        }
        int mid = (lx + rx) / 2;
        build(arr, ni * 2 + 1, lx, mid);
        build(arr, ni * 2 + 2, mid, rx);
        seg[ni] = merge(seg[ni * 2 + 1], seg[ni * 2 + 2]);
    }

    void build(vector<int> &arr)
    {
        build(arr, 0, 0, sz);
    }

    // 1. Elements Strictly Less Than K (< K)
    int query_less(int l, int r, int k, int ni, int lx, int rx)
    {
        if (lx >= l && rx <= r)
        {
            return lower_bound(all(seg[ni].v), k) - seg[ni].v.begin();
        }
        if (lx >= r || rx <= l)
            return 0;
        int mid = (lx + rx) / 2;
        return query_less(l, r, k, ni * 2 + 1, lx, mid) + query_less(l, r, k, ni * 2 + 2, mid, rx);
    }

    // 2. Elements Less Than or Equal K (<= K)
    int query_less_equal(int l, int r, int k, int ni, int lx, int rx)
    {
        if (lx >= l && rx <= r)
        {
            return upper_bound(all(seg[ni].v), k) - seg[ni].v.begin();
        }
        if (lx >= r || rx <= l)
            return 0;
        int mid = (lx + rx) / 2;
        return query_less_equal(l, r, k, ni * 2 + 1, lx, mid) + query_less_equal(l, r, k, ni * 2 + 2, mid, rx);
    }

    // 3. Elements Strictly Greater Than K (> K)
    int query_greater(int l, int r, int k, int ni, int lx, int rx)
    {
        if (lx >= l && rx <= r)
        {
            return seg[ni].v.end() - upper_bound(all(seg[ni].v), k);
        }
        if (lx >= r || rx <= l)
            return 0;
        int mid = (lx + rx) / 2;
        return query_greater(l, r, k, ni * 2 + 1, lx, mid) + query_greater(l, r, k, ni * 2 + 2, mid, rx);
    }

    // 4. Elements Greater Than or Equal K (>= K)
    int query_greater_equal(int l, int r, int k, int ni, int lx, int rx)
    {
        if (lx >= l && rx <= r)
        {
            return seg[ni].v.end() - lower_bound(all(seg[ni].v), k);
        }
        if (lx >= r || rx <= l)
            return 0;
        int mid = (lx + rx) / 2;
        return query_greater_equal(l, r, k, ni * 2 + 1, lx, mid) + query_greater_equal(l, r, k, ni * 2 + 2, mid, rx);
    }
    // 5. Elements in Range [X, Y]
    int query_in_range(int l, int r, int x, int y)
    {
        // Total elements <= Y  MINUS  Total elements < X
        return get_less_equal(l, r, y) - get_less(l, r, x);
    }

    // Helpers
    int get_greater(int l, int r, int k) { return query_greater(l, r, k, 0, 0, sz); }
    int get_greater_equal(int l, int r, int k) { return query_greater_equal(l, r, k, 0, 0, sz); }
    int get_less(int l, int r, int k) { return query_less(l, r, k, 0, 0, sz); }
    int get_less_equal(int l, int r, int k)
    {
        return query_less_equal(l, r, k, 0, 0, sz);
    }
};