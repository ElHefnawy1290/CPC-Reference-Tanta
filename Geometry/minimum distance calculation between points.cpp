// Finds the Closest Pair of Points in a 2D plane in O(N log N) time
// Uses a Sweep-line algorithm combined with a balanced BST (std::set)

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <iomanip>

using namespace std;

// Struct to hold point data, including original index for the final answer
struct Point {
    double x, y;
    int id; 
};

// 1. Sort primarily by X-coordinate to process points from left to right
bool compareX(const Point& a, const Point& b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

// 2. Custom comparator for the active window set to sort points by Y-coordinate
struct compareY {
    bool operator()(const Point& a, const Point& b) const {
        if (a.y != b.y) return a.y < b.y;
        return a.x < b.x; // Tie-breaker ensures points with same Y aren't treated as identical
    }
};

// Standard Euclidean distance
double get_dist(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void solve() {
    int n;
    if (!(cin >> n)) return;
    
    vector<Point> pts(n);
    for (int i = 0; i < n; i++) {
        cin >> pts[i].x >> pts[i].y;
        pts[i].id = i; // Save original 0-based index
    }

    // Sort all points from left to right for the sweep-line
    sort(pts.begin(), pts.end(), compareX);

    // Active window storing points currently within 'min_d' distance of the sweep line
    set<Point, compareY> active_set;
    
    double min_d = 1e18; // Start with a massive distance
    int best_i = -1, best_j = -1;
    
    int left = 0; // Left boundary pointer of our sweeping window
    
    for (int i = 0; i < n; i++) {
        // Step A: Shrink the window
        // Remove points too far left to possibly form a closer pair with the current point
        while (left < i && pts[i].x - pts[left].x >= min_d) {
            active_set.erase(pts[left]);
            left++;
        }
        
        // Step B: Check Y-bounds
        // Only check points in the active set whose Y is within [current_y - min_d, current_y + min_d]
        Point search_point = {pts[i].x, pts[i].y - min_d, -1};
        auto it = active_set.lower_bound(search_point); 
        
        while (it != active_set.end() && it->y - pts[i].y < min_d) {
            double current_d = get_dist(pts[i], *it);
            
            // Step C: Update if a strictly closer pair is found
            if (current_d < min_d) {
                min_d = current_d;
                best_i = pts[i].id;
                best_j = it->id;
            }
            it++;
        }
        
        // Step D: Add the current point into the active window for future comparisons
        active_set.insert(pts[i]);
    }
    
    // Ensure the smaller index is printed first
    if (best_i > best_j) swap(best_i, best_j);
    
    cout << best_i << " " << best_j << " " << fixed << setprecision(6) << min_d << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}