/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: DSU WITH ROLLBACKS & SEGMENT TREE OVER TIME
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Add and remove edges", "Dynamic Connectivity", "Offline queries", "Bipartite over time".
 * - Classic Scenarios: You have a graph and Q queries. Some queries ADD an edge, some REMOVE 
 *   an existing edge, and some ask for the number of connected components. Standard DSU 
 *   can only add edges, it CANNOT remove them!
 * - The Magic: "Time Travel using Segment Tree". Since we read all queries offline, we know the 
 *   exact "lifespan" of every edge (from time L when it was added, to time R when it was removed). 
 *   We treat a Segment Tree as a timeline. We insert each edge into the segment tree over the 
 *   range [L, R]. Then, we DFS through the segment tree. 
 *   Going DOWN: We add the edges using a modified DSU. 
 *   At the LEAVES: We are at a specific point in time! We answer the queries for that moment.
 *   Going UP: We UNDO (rollback) the edges we added. Because we strictly use Union-by-Size 
 *   (NO path compression), our DSU history is perfectly preserved and easily reversible!
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Create the segment tree. 
 *       segmentTree st(N); 
 *
 * - Add Edges: For every edge, find its active time range [L, R) and add it.
 *       st.set(L, R, edge(u, v));
 *
 * - Execution: Call the main DFS function to traverse time and answer queries.
 *       st.get();
 *
 * - Complexity:
 *       Time: O(Q log Q log N) — Every edge is split into log Q segments, and each DSU operation takes log N.
 *       Space: O(Q log Q) to store the edges in the segment tree nodes.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - The `N` vs `Q` Trap: In this current template, `segmentTree(int n)` uses the same `n` for 
 *   BOTH the number of nodes `d(n)` and the segment tree time span. Usually, Nodes (N) != Queries (Q). 
 *   You should change the constructor to: `segmentTree(int max_time, int max_nodes) : d(max_nodes)`.
 * - How to track L and R: Use a `map<pair<int,int>, int> active_edges`. When an edge is added at 
 *   time `t1`, store it. When it's removed at time `t2`, call `st.set(t1, t2, edge)`. Don't forget 
 *   to add the edges that are never removed until the end: `st.set(t1, Q, edge)`.
 * - Answering Queries: Go to the `if (rx - lx == 1)` block inside `get(...)`. At this exact line, 
 *   the DSU holds the exact state of the graph at time `lx`. You can print `d.get_forests()` 
 *   or check `d.same_set(u, v)` based on what query was asked at time `lx`.
 */

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
