// Counts pairs in subarrays where one element divides the other
// Uses Offline Query Processing and a Segment Tree in O((N + M) log N) time

#include <bits/stdc++.h>
using namespace std;

// Standard Point-Update, Range-Sum Segment Tree (1-based indexing)
struct SegmentTree {
    int n;
    vector<int> tree;
    
    SegmentTree(int n) {
        this->n = n;
        tree.assign(4 * n + 1, 0);
    }
    
    void add(int node, int start, int end, int idx, int val) {
        if (start == end) {
            tree[node] += val;
            return;
        }
        int mid = start + (end - start) / 2;
        if (idx <= mid) {
            add(2 * node, start, mid, idx, val);
        } else {
            add(2 * node + 1, mid + 1, end, idx, val);
        }
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }
    
    void add(int idx, int val) {
        add(1, 1, n, idx, val);
    }
    
    int query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0;
        
        if (l <= start && end <= r) return tree[node];
        
        int mid = start + (end - start) / 2;
        int left_sum = query(2 * node, start, mid, l, r);
        int right_sum = query(2 * node + 1, mid + 1, end, l, r);
        
        return left_sum + right_sum;
    }
    
    int query(int l, int r) {
        return query(1, 1, n, l, r);
    }
};

struct Query {
    int l, r, id;
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<int> p(n + 1);
    vector<int> pos(n + 1);
    
    for (int i = 1; i <= n; i++) {
        cin >> p[i];
        pos[p[i]] = i; // Store the 1-based index where each value is located
    }

    // pre-calculate all valid pairs (i, j) where i divides j
    // pairs_ending_at[R] will store the left index 'L' for a valid pair ending at index 'R'
    vector<vector<int>> pairs_ending_at(n + 1);
    for (int i = 1; i <= n; i++) {
        // Harmonic series loop: steps through all multiples of i
        // Runs in O(N log N) total time across all iterations
        for (int j = i; j <= n; j += i) {
            int u = pos[i];
            int v = pos[j];
            
            int L = min(u, v);
            int R = max(u, v);
            pairs_ending_at[R].push_back(L);
        }
    }

    vector<Query> queries(m);
    for (int i = 0; i < m; i++) {
        cin >> queries[i].l >> queries[i].r;
        queries[i].id = i;
    }

    // Offline Query Trick: Sort queries by their right endpoint in ascending order
    sort(queries.begin(), queries.end(), [](const Query& a, const Query& b) {
        return a.r < b.r;
    });

    vector<int> ans(m);
    SegmentTree segTree(n);

    int q_idx = 0;
    
    // Sweep through the array from left to right (simulating expanding the subarray)
    for (int r = 1; r <= n; r++) {
        // "Activate" all pairs whose rightmost element is exactly at the current 'r'
        for (int l : pairs_ending_at[r]) {
            segTree.add(l, 1);
        }

        // Answer all queries that ask for a subarray ending at the current 'r'
        while (q_idx < m && queries[q_idx].r == r) {
            int l = queries[q_idx].l;
            // Since all active pairs in the tree currently have their right endpoint <= r,
            // we just need to count how many of them have their left endpoint >= l.
            ans[queries[q_idx].id] = segTree.query(l, r);
            q_idx++;
        }
    }

    for (int i = 0; i < m; i++) {
        cout << ans[i] << "\n";
    }

    return 0;
}