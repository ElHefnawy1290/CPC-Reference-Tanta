#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 2e18; // Infinity for initializations

// A structure to represent the line y = mx + c
struct Line {
    ll m, c;
    // For Maximization, initialize empty lines with -INF. 
    // (If you need Minimization, change this to +INF).
    Line(ll m = 0, ll c = -INF) : m(m), c(c) {}
    
    ll eval(ll x) { 
        return m * x + c; 
    }
};

struct LiChaoTree {
    struct Node {
        Line line;
        int lc, rc; // Left child and Right child indices
        Node() : line(), lc(-1), rc(-1) {}
    };

    vector<Node> tree;
    ll min_x, max_x;

    // Initialize with your expected coordinate range (e.g., -1e9 to 1e9)
    LiChaoTree(ll min_x, ll max_x) : min_x(min_x), max_x(max_x) {
        tree.push_back(Node());
    }

    void add_line(Line nw, int u, ll l, ll r) {
        ll m = l + (r - l) / 2;
        
        // Change > to < if you want to solve a Minimization problem
        bool left_better = nw.eval(l) > tree[u].line.eval(l);
        bool mid_better = nw.eval(m) > tree[u].line.eval(m);

        // The line that is better at the midpoint \"owns\" this node
        if (mid_better) {
            swap(tree[u].line, nw);
        }

        if (r - l == 1) return; // Leaf node reached

        // If the old line was better at one of the boundaries, 
        // it means the lines intersect. Push the losing line down to the children!
        if (left_better != mid_better) {
            if (tree[u].lc == -1) {
                tree[u].lc = tree.size();
                tree.push_back(Node());
            }
            add_line(nw, tree[u].lc, l, m);
        } else {
            if (tree[u].rc == -1) {
                tree[u].rc = tree.size();
                tree.push_back(Node());
            }
            add_line(nw, tree[u].rc, m, r);
        }
    }

    void add_line(ll m, ll c) {
        add_line(Line(m, c), 0, min_x, max_x);
    }

    ll query(ll x, int u, ll l, ll r) {
        if (u == -1) return -INF; // Change to +INF for Minimization
        
        ll m = l + (r - l) / 2;
        ll cur = tree[u].line.eval(x);
        
        if (r - l == 1) return cur; // Leaf node reached
        
        // Change max() to min() for Minimization
        if (x < m) {
            return max(cur, query(x, tree[u].lc, l, m));
        } else {
            return max(cur, query(x, tree[u].rc, m, r));
        }
    }

    ll query(ll x) {
        return query(x, 0, min_x, max_x);
    }
};


How to use it:Initialize: Define the range of your $X$-coordinates (your $k$ queries).

// If your query values k range from -10^5 to 10^5:
LiChaoTree lct(-100005, 100005);

Add Lines: Loop through your pairs and insert the lines.// Insert line y = mx + c
lct.add_line(m, c);

Query: Find the maximum $y$ for a specific $x$.
ll max_y = lct.query(k);


add_line(m, c): Add a line to the set in $O(\log(\text{range}))$ time.
query(x): Find the maximum $y$ among all lines at coordinate $x$ in $O(\log(\text{range}))$ time.