// Distinct elements in range
// kth element
// couting or frequency of numbers in range

struct PST
{
    struct Node
    {
        int l = 0;
        int r = 0;
        int val = 0;
    };

    vector<Node> tree;
    vector<int> roots;
    int n;
    int neutral;

    PST(int n, int neutral = 0) : n(n), neutral(neutral)
    {
        tree.reserve(1500000);
        tree.push_back({0, 0, neutral});
        roots.push_back(0);
    }

    int merge(int a, int b)
    {
        return a + b;
    }

    int build(const vector<int> &a, int lx, int rx)
    {
        int u = tree.size();
        tree.push_back({0, 0, neutral});

        if (rx - lx == 1)
        {
            if (lx < (int)a.size())
                tree[u].val = a[lx];
            return u;
        }

        int mid = lx + (rx - lx) / 2;
        tree[u].l = build(a, lx, mid);
        tree[u].r = build(a, mid, rx);
        tree[u].val = merge(tree[tree[u].l].val, tree[tree[u].r].val);
        return u;
    }

    void build_initial(const vector<int> &a)
    {
        roots.push_back(build(a, 0, n));
    }

    int update(int node, int idx, int val, int lx, int rx)
    {
        int u = tree.size();
        tree.push_back(tree[node]);

        if (rx - lx == 1)
        {
            tree[u].val += val;
            return u;
        }

        int mid = lx + (rx - lx) / 2;
        if (idx < mid)
        {
            int nxt = update(tree[node].l, idx, val, lx, mid);
            tree[u].l = nxt;
        }
        else
        {
            int nxt = update(tree[node].r, idx, val, mid, rx);
            tree[u].r = nxt;
        }

        tree[u].val = merge(tree[tree[u].l].val, tree[tree[u].r].val);
        return u;
    }

    int update(int version, int idx, int val)
    {
        int new_root = update(roots[version], idx, val, 0, n);
        roots.push_back(new_root);
        return (int)roots.size() - 1;
    }

    int query(int node, int l, int r, int lx, int rx)
    {
        if (node == 0)
            return neutral;
        if (lx >= r || rx <= l)
            return neutral;
        if (lx >= l && rx <= r)
            return tree[node].val;

        int mid = lx + (rx - lx) / 2;
        return merge(query(tree[node].l, l, r, lx, mid),
                     query(tree[node].r, l, r, mid, rx));
    }

    int query(int version, int l, int r)
    {
        return query(roots[version], l, r, 0, n);
    }

    int findKth(int l, int r, int k, int lx, int rx)
    {
        if (rx - lx == 1)
            return lx;
        int cnt = tree[tree[r].l].val - tree[tree[l].l].val;
        int mid = lx + (rx - lx) / 2;
        if (cnt >= k)
            return findKth(tree[l].l, tree[r].l, k, lx, mid);
        return findKth(tree[l].r, tree[r].r, k - cnt, mid, rx);
    }

    int findKth(int l, int r, int k)
    {
        return findKth(roots[l], roots[r], k, 0, n);
    }
};
