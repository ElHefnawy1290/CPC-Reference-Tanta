/ Detects if ANY two line segments intersect in a set of N segments
// Uses a simplified Bentley-Ottmann Sweep-line Algorithm in O(N log N) time

#include <bits/stdc++.h>
using namespace std;
 
#define int long long
#define endl '\n'
 
// 1. Struct for exact integer math (Zero precision issues!)
struct pt {
    int x, y;
};
 
// Standard 2D cross product
int cross(pt a, pt b, pt c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}
 
int sgn(int val) {
    return (val > 0) - (val < 0);
}
 
// 1D bounding box check
bool intersect1D(int a, int b, int c, int d) {
    if (a > b) swap(a, b);
    if (c > d) swap(c, d);
    return max(a, c) <= min(b, d);
}
 
// Standard integer exact segment intersection
// Checks bounding boxes first, then uses cross products to ensure segments straddle each other
bool check_intersect(pt a, pt b, pt c, pt d) {
    return intersect1D(a.x, b.x, c.x, d.x) &&
           intersect1D(a.y, b.y, c.y, d.y) &&
           sgn(cross(a, b, c)) * sgn(cross(a, b, d)) <= 0 &&
           sgn(cross(c, d, a)) * sgn(cross(c, d, b)) <= 0;
}
 
// 2. Floating point struct just for ordering the Sweep Line
struct pt_f {
    double x, y;
};
 
struct seg {
    pt_f p, q;
    int id;
    pt orig_p, orig_q; // Keep original points to do the exact integer check
    
    // Get the Y coordinate of the segment at a specific sweep line X
    double get_y(double x) const {
        if (abs(p.x - q.x) < 1e-9) return p.y;
        return p.y + (q.y - p.y) * (x - p.x) / (q.x - p.x);
    }
};
 
double current_x;
 
// Custom comparator for the active window (Sweep Line)
// Maintains the vertical order of segments at the current X coordinate
struct cmp {
    bool operator()(const seg& a, const seg& b) const {
        double ya = a.get_y(current_x);
        double yb = b.get_y(current_x);
        if (abs(ya - yb) > 1e-9) return ya < yb;
        return a.id < b.id; // Tie-breaker
    }
};
 
struct event {
    double x;
    int type; // 1 for left endpoint (insert), -1 for right endpoint (remove)
    seg s;
    bool operator<(const event& e) const {
        if (abs(x - e.x) > 1e-9) return x < e.x;
        return type > e.type; // Process left endpoints before right ones if X is tied
    }
};
 
void solve() {
    int n;
    if (!(cin >> n)) return;
 
    vector<event> events;
    vector<seg> segments(n);
 
    // Trick: Rotate coordinates slightly to avoid vertical line nightmares.
    // Vertical lines break the y=mx+b sweep line logic because multiple points share the same X.
    double angle = 0.001;
    double sinA = sin(angle), cosA = cos(angle);
 
    for (int i = 0; i < n; i++) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        
        segments[i].orig_p = {x1, y1};
        segments[i].orig_q = {x2, y2};
        segments[i].id = i + 1; // 1-based index for the output
 
        // Apply rotation to the floating-point representations
        pt_f p = {x1 * cosA - y1 * sinA, x1 * sinA + y1 * cosA};
        pt_f q = {x2 * cosA - y2 * sinA, x2 * sinA + y2 * cosA};
 
        // Always sweep left to right
        if (p.x > q.x) swap(p, q);
 
        segments[i].p = p;
        segments[i].q = q;
 
        // Create left and right events for the sweep line
        events.push_back({p.x, 1, segments[i]});
        events.push_back({q.x, -1, segments[i]});
    }
 
    sort(events.begin(), events.end());
 
    set<seg, cmp> active;
    vector<set<seg>::iterator> where(n + 1); // Keep track of elements in the set for quick deletion
 
    for (auto& ev : events) {
        current_x = ev.x;
        int id = ev.s.id;
 
        if (ev.type == 1) { 
            // 1. Insert segment into the active window
            auto nxt = active.insert(ev.s).first;
            where[id] = nxt;
            
            // 2. Check if it intersects with the segment immediately above or below it
            auto prv = nxt, nxt2 = nxt;
            if (prv != active.begin()) {
                prv--;
                if (check_intersect(ev.s.orig_p, ev.s.orig_q, prv->orig_p, prv->orig_q)) {
                    cout << "YES\n" << ev.s.id << " " << prv->id << "\n";
                    return;
                }
            }
            nxt2++;
            if (nxt2 != active.end()) {
                if (check_intersect(ev.s.orig_p, ev.s.orig_q, nxt2->orig_p, nxt2->orig_q)) {
                    cout << "YES\n" << ev.s.id << " " << nxt2->id << "\n";
                    return;
                }
            }
        } else {
            // 1. Find the segment in the active window
            auto it = where[id];
            
            // 2. Before removing it, check if its top and bottom neighbors now intersect
            auto prv = it, nxt = it;
            nxt++;
            if (prv != active.begin() && nxt != active.end()) {
                prv--;
                if (check_intersect(prv->orig_p, prv->orig_q, nxt->orig_p, nxt->orig_q)) {
                    cout << "YES\n" << prv->id << " " << nxt->id << "\n";
                    return;
                }
            }
            // 3. Remove the segment
            active.erase(it);
        }
    }
    
    // If the sweep line finishes with no hits
    cout << "NO\n";
}