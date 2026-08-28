/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: LI CHAO TREE (EXTENDED)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Minimum of lines", "Linear cost over a range", "y = mx + c".
 * - Classic Scenarios: You need to minimize a linear DP transition cost, but
 *   certain transitions/lines are only valid within a specific interval [L, R].
 * - The Magic: A sparse segment tree where each node holds the line that
 *   provides the optimal value at the midpoint of its interval. It dynamically
 *   allocates memory (sparse) so it can comfortably handle x-coordinates up to 10^18.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization:
 *       LiChaoTree lct(-1e18, 1e18); // Pass the minimum and maximum possible X queries
 * - Operations:
 *       lct.add_line(M, C);                  // Adds an infinite line y = Mx + C
 *       lct.add_segment(M, C, L, R);         // Adds line y = Mx + C strictly for x in [L, R]
 *       long long min_y = lct.query(X);      // Gets the minimum y at point X
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Maximization: This template is strictly built to find the MINIMUM value.
 *   If you need the MAXIMUM, insert your lines with inverted signs:
 *   `lct.add_line(-M, -C)` and then negate the result: `-lct.query(X)`.
 */

using ll = long long;
const ll INF = 4e18; // Safe upper bound to prevent overflow during additions

struct Line
{
    ll m, c;
    ll eval(ll x) const { return m * x + c; }
};

struct Node
{
    Line line;
    int lc = -1, rc = -1;
    Node() : line({0, INF}) {}
    Node(Line l) : line(l) {}
};

struct LiChaoTree
{
    vector<Node> tree;
    ll MIN_X, MAX_X;

    LiChaoTree(ll min_x, ll max_x) : MIN_X(min_x), MAX_X(max_x)
    {
        tree.emplace_back();
    }

    void add_line(Line nw, int u, ll l, ll r)
    {
        ll mid = l + (r - l) / 2;
        bool left = nw.eval(l) < tree[u].line.eval(l);
        bool middle = nw.eval(mid) < tree[u].line.eval(mid);

        if (middle)
            swap(tree[u].line, nw);
        if (l == r)
            return;

        if (left != middle)
        {
            if (tree[u].lc == -1)
            {
                tree[u].lc = tree.size();
                tree.emplace_back();
            }
            add_line(nw, tree[u].lc, l, mid);
        }
        else
        {
            if (tree[u].rc == -1)
            {
                tree[u].rc = tree.size();
                tree.emplace_back();
            }
            add_line(nw, tree[u].rc, mid + 1, r);
        }
    }

    void add_line(ll m, ll c)
    {
        add_line({m, c}, 0, MIN_X, MAX_X);
    }

    void add_segment(Line nw, ll ql, ll qr, int u, ll l, ll r)
    {
        if (ql <= l && r <= qr)
        {
            add_line(nw, u, l, r);
            return;
        }
        ll mid = l + (r - l) / 2;
        if (ql <= mid)
        {
            if (tree[u].lc == -1)
            {
                tree[u].lc = tree.size();
                tree.emplace_back();
            }
            add_segment(nw, ql, qr, tree[u].lc, l, mid);
        }
        if (qr > mid)
        {
            if (tree[u].rc == -1)
            {
                tree[u].rc = tree.size();
                tree.emplace_back();
            }
            add_segment(nw, ql, qr, tree[u].rc, mid + 1, r);
        }
    }

    void add_segment(ll m, ll c, ll ql, ll qr)
    {
        add_segment({m, c}, ql, qr, 0, MIN_X, MAX_X);
    }

    ll query(ll x, int u, ll l, ll r)
    {
        if (u == -1)
            return INF;
        ll res = tree[u].line.eval(x);
        if (l == r)
            return res;

        ll mid = l + (r - l) / 2;
        if (x <= mid)
            res = min(res, query(x, tree[u].lc, l, mid));
        else
            res = min(res, query(x, tree[u].rc, mid + 1, r));

        return res;
    }

    ll query(ll x)
    {
        return query(x, 0, MIN_X, MAX_X);
    }
};