/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: HEAVY-LIGHT DECOMPOSITION (HLD)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Path queries on a tree", "Subtree queries with point updates", "O(log^2 N) tree path".
 * - Classic Scenarios: You have a tree with values on the nodes. You need to answer $Q$ queries 
 *   that either update a node's value OR ask for the maximum/sum on the simple path between $U$ and $V$. 
 *   Standard DFS/BFS takes $O(N)$ per query, which gives TLE. 
 * - The Magic: "Tree Flattening into 1D". HLD cuts the tree into a set of disjoint vertical paths 
 *   ("heavy chains"). It then maps these chains into a 1D array so we can build a Segment Tree over it. 
 *   The mathematical guarantee is beautiful: ANY path between two nodes in the tree will cross 
 *   at most $O(\log N)$ heavy chains. Thus, a path query becomes just $O(\log N)$ segment tree queries!
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Pass the number of nodes, the adjacency list (0-indexed or 1-indexed safely), 
 *   and a 1-indexed vector of initial node values.
 *       // Note: Define GRAPH as vector<vector<int>> beforehand
 *       HLD hld(N, adj, initial_values);
 *
 * - Point Update: Change the value of node U to X.
 *       hld.point_update(U, X);
 *
 * - Path Query: Get the maximum value on the path from U to V.
 *       int max_val = hld.query_path(U, V);
 *
 * - Subtree Query: Get the maximum value in the entire subtree of U.
 *       int sub_max = hld.query_subtree(U);
 *
 * - Complexity:
 *       Time: Build $O(N)$, Updates $O(\log N)$, Subtree Queries $O(\log N)$, Path Queries $O(\log^2 N)$.
 *       Space: $O(N)$ for the arrays and Segment Tree.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Sum/Min instead of Max? 
 *   1. Change the identity value in the `Node` constructor (e.g., `-OO` to `0` for sum, or `OO` for min).
 *   2. Change `max(lf.val, rt.val)` in `merge()` to `+` or `min`.
 *   3. Inside `HLD::query_path`, change `int res{-OO};` and `res = max(...)` to match your operation.
 * - Edges instead of Nodes?
 *   If weights are on edges, push the edge weight down to the deeper node (the child). 
 *   Then, in `query_path`, when querying the final segment between the LCA and the other node, 
 *   you must EXCLUDE the LCA (because the LCA holds the weight of the edge above it, which is not on the path). 
 *   Change `seg.get(tin[v], tin[u] + 1)` to `seg.get(tin[v] + 1, tin[u] + 1)`.
 * - GRAPH Typedef: Ensure you have `typedef vector<vector<int>> GRAPH;` globally so the constructor compiles.
 */
struct Node
{
    int val;

    Node()
    {
        val = -OO;
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

struct HLD
{
private:
    vector<int> sz, par, dep, tin, tout, head;
    vector<vector<int>> graph;
    vector<int> a;
    int timer{1};
    segmentTree seg;

public:
    HLD(int n, GRAPH graph, vector<int> &v) : graph(std::move(graph)), seg(n + 1)
    {
        sz = vector<int>(n + 1), par = vector<int>(n + 1), dep = vector<int>(n + 1), tin = vector<int>(n + 1), tout = vector<int>(n + 1), head = vector<int>(n + 1), a = vector<int>(n + 1);
        dfs1(1, 0);
        head[1] = 1;
        dfs2(1);
        for (int i = 1; i <= n; i++)
            a[tin[i]] = v[i];
        seg.init(a);
    }

    void dfs1(int u, int p)
    {
        dep[u] = dep[p] + 1;
        par[u] = p;
        sz[u] = 1;
        if (p != 0)
            graph[u].erase(find(graph[u].begin(), graph[u].end(), p));
        for (auto &x : graph[u])
        {
            dfs1(x, u);
            sz[u] += sz[x];
            if (sz[x] > sz[graph[u][0]])
                swap(x, graph[u][0]);
        }
    }

    void dfs2(int u)
    {
        tin[u] = timer++;
        for (auto &x : graph[u])
        {
            head[x] = x == graph[u][0] ? head[u] : x;
            dfs2(x);
        }
        tout[u] = timer;
    }

    int lca(int u, int v)
    {
        while (head[u] != head[v])
        {
            if (dep[head[u]] > dep[head[v]])
                u = par[head[u]];
            else
                v = par[head[v]];
        }
        if (dep[u] < dep[v])
            return u;
        return v;
    }

    void point_update(int u, int val)
    {
        seg.set(tin[u], val);
    }

    int query_subtree(int u)
    {
        return seg.get(tin[u], tout[u]);
    }

    // now querying for maximum
    int query_path(int u, int v)
    {
        int res{-OO}; // TO
        while (head[u] != head[v])
        {
            if (dep[head[u]] < dep[head[v]])
                swap(u, v);
            res = max(res, seg.get(tin[head[u]], tin[u] + 1)); // TO
            u = par[head[u]];
        }
        if (dep[u] < dep[v])
            swap(u, v);
        res = max(res, seg.get(tin[v], tin[u] + 1)); // TO
        return res;
    }
};
