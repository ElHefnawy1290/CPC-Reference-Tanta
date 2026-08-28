class DSU
{
private:
    vector<int> parent, size;
    stack<int> changes;
    int forests{};

    void link(int x, int y)
    {
        if (size[x] > size[y])
            swap(x, y);
        changes.push(x);
        parent[x] = y;
        size[y] += size[x];
    }

public:
    DSU(int n)
    {
        parent = vector<int>(n), size = vector<int>(n), changes = stack<int>();
        forests = n;
        for (int i = 0; i < n; i++)
            parent[i] = i, size[i] = 1;
    }

    int find_set(int x)
    {
        while (x != parent[x])
            x = parent[x];
        return x;
    }

    int union_sets(int x, int y)
    {
        x = find_set(x), y = find_set(y);
        if (x != y)
        {
            link(x, y);
            forests--;
        }
        return x != y;
    }

    void rollback()
    {
        if (changes.empty())
            return;
        int x = changes.top();
        changes.pop();
        size[parent[x]] -= size[x];
        parent[x] = x;
        forests++;
    }

    bool same_set(int x, int y)
    {
        x = find_set(x), y = find_set(y);
        return x == y;
    }

    int get_size(int x)
    {
        return size[find_set(x)];
    }

    int get_forests() const
    {
        return forests;
    }
};

struct edge
{
    ll from, to;

    edge()
    {
    }

    edge(ll from, ll to) : from(from), to(to)
    {
    }

    bool operator==(const edge &e)
    {
        return from == e.from && to == e.to;
    }
};

struct Node
{
    int val;

    Node()
    {
        val = 0;
    }

    Node(int val) : val(val)
    {
    }

    void change(int x)
    {
        val = x;
    }
};

struct segmentTree
{
    int sz;
    vector<Node> seg;
    vector<vector<edge>> edges;
    DSU d;

    segmentTree(int n) : d(n)
    {
        sz = 1;
        while (sz < n)
            sz *= 2;
        seg.assign(2 * sz, Node());
        edges.assign(2 * sz, {});
    }

    void set(int l, int r, int node, int lx, int rx, edge edge)
    {
        if (lx >= l && rx <= r)
        {
            edges[node].push_back(edge);
            return;
        }
        if (rx <= l || lx >= r)
            return;
        int mid = (lx + rx) / 2;
        set(l, r, 2 * node + 1, lx, mid, edge);
        set(l, r, 2 * node + 2, mid, rx, edge);
    }

    void set(int l, int r, edge edge)
    {
        set(l, r, 0, 0, sz, edge);
    }

    void get(int node, int lx, int rx)
    {
        int cnt{};
        for (auto &[x, y] : edges[node])
            cnt += d.union_sets(x, y);
        if (rx - lx == 1)
        {
            // answer queries
        }
        else
        {
            int mid = (lx + rx) / 2;
            get(2 * node + 1, lx, mid);
            get(2 * node + 2, mid, rx);
        }
        while (cnt--)
            d.rollback();
    }

    void get()
    {
        return get(0, 0, sz);
    }
};
