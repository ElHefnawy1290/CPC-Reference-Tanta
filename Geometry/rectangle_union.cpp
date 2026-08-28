// To find the total area covered by a union of given rectangles
// Uses a Sweep-line algorithm paired with a Lazy Segment Tree

#include <bits/stdc++.h>
 
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
#define endl '\n'
#define all(a)   a.begin(),a.end()
#define all_r(a)   a.rbegin(),a.rend()
#define int ll
 
const int oo = 1e9;
 
struct segtree{
    struct Node{
        int mn, freqmn, lazy, isLazy;
        Node(){
            mn = oo, freqmn = 0;
            isLazy = 0;
            lazy = 0;
        }
        void change(int x, int lx, int rx){
            mn += x;
            lazy += x;
            isLazy = 1;
        }
        Node(int x){
            mn = x;
            freqmn = 1; // Represents the length of this minimum value's segment
            isLazy = 0;
            lazy = 0;
        }
    };
 
    vector<Node> segData;
    int treeSize;
 
    void propagate(int nx, int lx, int rx){
        if(!segData[nx].isLazy || rx - lx == 1)
            return;
        int mid = lx + (rx - lx)/2;
        segData[2 * nx + 1].change(segData[nx].lazy, lx, mid);
        segData[2 * nx + 2].change(segData[nx].lazy, mid, rx);
        segData[nx].lazy = segData[nx].isLazy = 0;
    }
 
    segtree(int n){
        treeSize = 1;
        while(treeSize < n)
            treeSize *= 2;
        segData.assign(2 * treeSize, Node());
    }
 
    Node merge(Node lf, Node ri){
        Node ans = Node();
        ans.mn = min(lf.mn, ri.mn);
        // Combine frequencies of the minimums to track how much of the range is uncovered
        if(lf.mn == ans.mn) ans.freqmn += lf.freqmn;
        if(ri.mn == ans.mn) ans.freqmn += ri.freqmn;
        return ans;
    }
 
    void set(int l, int r, int val, int nx, int lx, int rx){
        propagate(nx, lx, rx);
        if(rx <= l || lx >= r) return;
        if(lx >= l && rx <= r){
            segData[nx].change(val, lx, rx);
            return;
        }
 
        int mid = lx + (rx - lx)/2;
 
        set(l, r, val, 2 * nx + 1, lx, mid);
        set(l, r, val, 2 * nx + 2, mid, rx);
        segData[nx] = merge(segData[2 * nx + 1], segData[2 * nx + 2]);
    }
 
    void set(int l, int r, int val){
        set(l, r, val, 0, 0, treeSize);
    }
 
    Node get(int l, int r, int nx, int lx, int rx){
        propagate(nx, lx, rx);
        if(l <= lx && r >= rx) return segData[nx];
        if(r <= lx || l >= rx) return Node();
 
        int mid = lx + (rx - lx)/2;
        return merge(get(l, r, 2 * nx + 1, lx, mid) ,get(l, r, 2 * nx + 2, mid, rx));
    }
 
    // Returns the total length of the active Y-intervals
    int get(int l, int r){
        Node ans = get(l, r, 0, 0, treeSize);
        // If the minimum is 0, subtract the frequency of 0s (uncovered space) from total range
        // If the minimum > 0, the entire range is completely covered by at least one rectangle
        return (r - l) - (ans.mn ? 0 : ans.freqmn);
    }
 
    void Build(vector<int> & v, int nx, int lx, int rx){
        if(rx - lx == 1){
            if(lx < v.size()){
                segData[nx]= Node(v[lx]);
            }
            return;
        }
 
        int mid = lx + (rx - lx)/2;
 
        Build(v, 2 * nx + 1, lx, mid);
        Build(v, 2 * nx + 2, mid, rx);
 
        segData[nx] = merge(segData[2 * nx + 1], segData[2 * nx + 2]);
    }
 
    void Build(vector<int> & v){
        Build(v, 0, 0, treeSize);
    }
};
 
// Represents a vertical line of a rectangle
struct event{
    int x, t, y1, y2; // x: coordinate, t: 1 for left edge / 0 for right edge
};
 
const int N = 2e6 + 1, sh = 1e6; // 'sh' offsets negative Y coordinates

void solve() {
    int n; cin >> n;
    vector<event> lines(2 * n);
    for (int i = 0; i < 2 * n; i += 2) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        // Map Y coordinates to positive values using shift 'sh'
        lines[i] = { x1, 1, y1 + sh, y2 + sh};
        lines[i + 1] = {x2, 0, y1 + sh, y2 + sh};
    }
 
    // Sort events from left to right. Process left edges before right edges if at the same X.
    sort(all(lines), [&](event & a, event & b)->bool{
        if (a.x != b.x) return a.x < b.x;
        return a.t < b.t;
    });
 
    int ans = 0;
 
    vector<int> b(N);
    segtree seg(N);
    seg.Build(b);
 
    int lastx = -1;
    
    for(auto& ev : lines){
        int x = ev.x;
        int t = ev.t;
        int y1 = ev.y1;
        int y2 = ev.y2;
        
        // Add (width * active_height) to total area
        ans += seg.get(0, N) * (x - lastx);
        
        // If it's a left edge, add 1 to the interval. If right edge, subtract 1.
        if(t) seg.set(y1, y2, 1);
        else seg.set(y1, y2, -1);
        
        lastx = x;
    }
 
    cout <<  ans << endl;
}
 
signed main() {
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    cout << fixed << setprecision(9);
    int test = 1; //cin>>test;
    while (test--) solve();
    
    return 0;
}
