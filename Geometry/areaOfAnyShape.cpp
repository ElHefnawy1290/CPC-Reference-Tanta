// Calculates the area of any simple polygon (convex or concave)
// Uses the Shoelace Formula (Surveyor's formula)

typedef long long ll;
typedef pair<ll, ll> pt;

// Computes signed area × 2 (keeps operations in integer math to avoid precision loss)
ll twiceArea(const vector<pt> &p) {
    int n = p.size();
    ll area = 0;
    for (int i = 0; i < n; i++) {
        ll x1 = p[i].first, y1 = p[i].second;
        // Wrap around to the first point when i is at the last vertex
        ll x2 = p[(i + 1) % n].first, y2 = p[(i + 1) % n].second;  
        
        // Accumulate the 2D cross product of adjacent vertices
        area += (x1 * y2 - x2 * y1);
    }
    // Return absolute value so it works regardless of clockwise or counter-clockwise input
    return abs(area);
}

void solve() {
    int n;
    cin >> n;
    vector<pt> p(n);
    for (int i = 0; i < n; ++i)
        cin >> p[i].first >> p[i].second;

    ll twice = twiceArea(p);
    // Divide by 2.0 at the very end to get the actual floating-point area
    cout << fixed << setprecision(1) << (twice / 2.0) << '\n';
}