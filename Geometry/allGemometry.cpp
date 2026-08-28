#include <iostream>
#include <complex>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <cassert>
#include <deque>
#include <numeric>

using namespace std;

// =============================================================================
//                              BASICS & TYPES
// =============================================================================

typedef long double ld;
typedef ld T;
typedef complex<T> pt;

const T EPS = 1e-9;
const long double PI = acos(-1.0);

#define x real()
#define y imag()

bool operator==(pt a, pt b) { return fabs(a.x - b.x) < EPS && fabs(a.y - b.y) < EPS; }
bool operator!=(pt a, pt b) { return !(a == b); }

int sgn(T val) {
    if (val > EPS) return 1;
    if (val < -EPS) return -1;
    return 0;
}

T sq(pt p) { return p.x * p.x + p.y * p.y; }
ld abs(pt p) { return sqrt(sq(p)); }

pt perp(pt p) { return {-p.y, p.x}; }

T dot(pt v, pt w) { return v.x * w.x + v.y * w.y; }
T cross(pt v, pt w) { return v.x * w.y - v.y * w.x; }

bool isPerp(pt v, pt w) { return fabs(dot(v, w)) < EPS; }

pt unitVector(pt p) {
    ld len = abs(p);
    if (len < EPS) return {0, 0};
    return p / len;
}

// =============================================================================
//                              TRANSFORMATIONS
// =============================================================================

pt translate(pt v, pt p) { return p + v; }

// Scale point p by a factor around center c
pt scale(pt c, T factor, pt p) {
    return c + (p - c) * factor;
}

// Rotate point p by angle a around center c
pt rot(pt p, pt c, ld a) {
    pt v = p - c;
    pt rotate = {cos(a), sin(a)};
    return c + rotate * v;
}

// If point p maps to fp and q maps to fq, find the image of r
pt linearTransfo(pt p, pt q, pt r, pt fp, pt fq) {
    return fp + (r - p) * (fq - fp) / (q - p);
}

// =============================================================================
//                              ANGLES
// =============================================================================

// (AB X AC) --> relative to AB: if(C left) return pos, else if (C right) return neg
T orient(pt a, pt b, pt c) { return cross(b - a, c - a); }

bool inAngle(pt a, pt b, pt c, pt p) {
    T abp = orient(a, b, p), acp = orient(a, c, p), abc = orient(a, b, c);
    if (abc < 0) swap(abp, acp);
    return (abp >= 0 && acp <= 0) ^ (abc < 0);
}

T angle(pt v, pt w) {
    return atan2(fabs(cross(v, w)), dot(v, w));
}

ld orientedAngle(pt a, pt b, pt c) {
    if (orient(a, b, c) >= 0)
        return angle(b - a, c - a);
    else
        return 2 * PI - angle(b - a, c - a);
}

ld angleTravelled(pt a, pt p, pt q) {
    ld ampli = angle(p - a, q - a);
    if (orient(a, p, q) > 0) return ampli;
    else return -ampli;
}

bool half(pt p) {
    return p.y > EPS || (fabs(p.y) <= EPS && p.x < -EPS);
}

// =============================================================================
//                              LINES
// =============================================================================

struct line {
    pt v; T c;

    // From direction vector v and offset c
    line(pt v, T c) : v(v), c(c) {}

    // From equation ax+by=c
    line(T a, T b, T _c) {
        v = {b, -a};
        c = _c;
    }

    // From points P and Q
    line(pt p, pt q) {
        v = q - p;
        c = cross(v, p);
    }

    T side(pt p) { return cross(v, p) - c; }
    double dist(pt p) { return abs(side(p)) / abs(v); }
    double sqDist(pt p) { return side(p) * side(p) / (T)sq(v); }
    line perpThrough(pt p) { return {p, p + perp(v)}; }
    bool cmpProj(pt p, pt q) { return dot(v, p) < dot(v, q); }
    line translate(pt t) { return {v, c + cross(v, t)}; }
    line shiftLeft(double dist) { return {v, c + dist * abs(v)}; }
    pt proj(pt p) { return p - perp(v) * side(p) / sq(v); }
    pt refl(pt p) { return p - perp(v) * (T)2.0 * side(p) / sq(v); }
};

bool inter(line l1, line l2, pt &out) {
    T d = cross(l1.v, l2.v);
    if (fabs(d) <= EPS) return false;
    out = (l2.v * l1.c - l1.v * l2.c) / d;
    return true;
}

line bisector(line l1, line l2, bool interior) {
    assert(cross(l1.v, l2.v) != 0); // Cannot be parallel
    T sign = interior ? 1 : -1;
    return {l2.v / (T)abs(l2.v) + l1.v / (T)abs(l1.v) * sign,
            l2.c / abs(l2.v) + l1.c / abs(l1.v) * sign};
}

// =============================================================================
//                              SEGMENTS
// =============================================================================

bool inDisk(pt a, pt b, pt p) {
    return dot(a - p, b - p) <= EPS;
}

bool onSegment(pt a, pt b, pt p) {
    return fabs(orient(a, b, p)) <= EPS && inDisk(a, b, p);
}

bool properInter(pt a, pt b, pt c, pt d, pt &out) {
    T oa = orient(c, d, a), ob = orient(c, d, b);
    T oc = orient(a, b, c), od = orient(a, b, d);
    if (sgn(oa) * sgn(ob) < 0 && sgn(oc) * sgn(od) < 0) {
        out = (a * ob - b * oa) / (ob - oa);
        return true;
    }
    return false;
}

set<pair<ld, ld>> inters(pt a, pt b, pt c, pt d) {
    set<pair<ld, ld>> s;
    pt out;
    if (a == c || a == d) s.insert({a.x, a.y});
    if (b == c || b == d) s.insert({b.x, b.y});
    if (s.size()) return s;

    if (properInter(a, b, c, d, out)) return {{out.x, out.y}};
    if (onSegment(c, d, a)) s.insert({a.x, a.y});
    if (onSegment(c, d, b)) s.insert({b.x, b.y});
    if (onSegment(a, b, c)) s.insert({c.x, c.y});
    if (onSegment(a, b, d)) s.insert({d.x, d.y});

    return s;
}

ld segPoint(pt a, pt b, pt p) {
    if (a != b) {
        line l(a, b);
        if (l.cmpProj(a, p) && l.cmpProj(p, b)) return l.dist(p);
    }
    return min(abs(p - a), abs(p - b));
}

ld rayPoint(pt a, pt b, pt p) {
    pt ab = b - a, ap = p - a;
    if (dot(ab, ap) < 0) return abs(ap);
    line l(a, b);
    return l.dist(p);
}

ld segSeg(pt a, pt b, pt c, pt d) {
    pt dummy;
    if (properInter(a, b, c, d, dummy)) return 0;
    return min({segPoint(a, b, c), segPoint(a, b, d),
                segPoint(c, d, a), segPoint(c, d, b)});
}

// =============================================================================
//                              CIRCLES
// =============================================================================

pair<pt, T> circumCircle(pt a, pt b, pt c) {
    b = b - a, c = c - a;
    assert(cross(b, c) != 0); // No circumcircle if aligned
    return {a + perp(b * sq(c) - c * sq(b)) / cross(b, c) / (T)2, 
            abs(perp(b * sq(c) - c * sq(b)) / cross(b, c) / (T)2)};
}

int circleLine(pt o, double r, line l, pair<pt, pt> &out) {
    double h2 = r * r - l.sqDist(o);
    if (h2 >= 0) {
        pt p = l.proj(o);
        pt h = l.v * (T)(sqrt(h2) / abs(l.v));
        out = {p - h, p + h};
    }
    return 1 + sgn(h2);
}

int circleCircle(pt o1, T r1, pt o2, T r2, pair<pt, pt> &out) {
    pt d = o2 - o1; T d2 = sq(d);
    if (d2 == 0) { assert(r1 != r2); return 0; }
    T pd = (d2 + r1 * r1 - r2 * r2) / 2;
    T h2 = r1 * r1 - pd * pd / d2;
    if (h2 >= 0) {
        pt p = o1 + d * pd / d2, h = perp(d) * sqrt(h2 / d2);
        out = {p - h, p + h};
    }
    return 1 + sgn(h2);
}

int tangents(pt o1, T r1, pt o2, T r2, bool inner, vector<pair<pt, pt>> &out) {
    if (inner) r2 = -r2;
    pt d = o2 - o1;
    T dr = r1 - r2, d2 = sq(d), h2 = d2 - dr * dr;
    if (d2 == 0 || h2 < 0) { assert(h2 != 0); return 0; }
    for (T sign : {-1, 1}) {
        pt v = (d * dr + perp(d) * sqrt(h2) * sign) / d2;
        out.push_back({o1 + v * r1, o2 + v * r2});
    }
    return 1 + (h2 > 0);
}

// =============================================================================
//                              BASIC POLYGONS
// =============================================================================

ld areaTriangle(pt a, pt b, pt c) { return abs(cross(b - a, c - a)) / 2.0; }

ld areaPolygon(vector<pt> p) {
    ld area = 0.0;
    for (int i = 0, n = p.size(); i < n; i++)
        area += cross(p[i], p[(i + 1) % n]);
    return abs(area) / 2.0;
}

bool isConvex(vector<pt> p) {
    bool hasPos = false, hasNeg = false;
    for (int i = 0, n = p.size(); i < n; i++) {
        int o = sgn(orient(p[i], p[(i + 1) % n], p[(i + 2) % n]));
        if (o > 0) hasPos = true;
        if (o < 0) hasNeg = true;
    }
    return !(hasPos && hasNeg);
}

bool above(pt a, pt p) { return p.y >= a.y; }

bool crossesRay(pt a, pt p, pt q) {
    return (above(a, q) - above(a, p)) * orient(a, p, q) > 0;
}

bool inPolygon(vector<pt> p, pt a, bool strict = true) {
    int numCrossings = 0;
    for (int i = 0, n = p.size(); i < n; i++) {
        if (onSegment(p[i], p[(i + 1) % n], a)) return !strict;
        numCrossings += crossesRay(a, p[i], p[(i + 1) % n]);
    }
    return numCrossings & 1;
}

bool onPolygonSide(vector<pt> p, pt a) {
    for (int i = 0, n = p.size(); i < n; i++) {
        if (onSegment(p[i], p[(i + 1) % n], a)) return true;
    }
    return false;
}

pt polygonCentroid(const vector<pt>& p) {
    int n = p.size();
    ld area = 0.0, cx = 0.0, cy = 0.0;
    for (int i = 0; i < n; i++) {
        int next = (i + 1) % n;
        ld cross_val = cross(p[i], p[next]);
        area += cross_val;
        cx += (p[i].x + p[next].x) * cross_val;
        cy += (p[i].y + p[next].y) * cross_val;
    }
    area /= 2.0;
    return {cx / (6.0 * area), cy / (6.0 * area)};
}

// =============================================================================
//                              PICK'S THEOREM
// =============================================================================

int boundaryPoints(const vector<pt>& p) {
    int b = 0, n = p.size();
    for (int i = 0; i < n; i++) {
        int next = (i + 1) % n;
        int dx = abs(p[i].x - p[next].x);
        int dy = abs(p[i].y - p[next].y);
        b += std::gcd(dx, dy);
    }
    return b;
}

int doubleAreaPolygon(const vector<pt>& p) {
    int doubleArea = 0, n = p.size();
    for (int i = 0; i < n; i++) {
        int next = (i + 1) % n;
        doubleArea += cross(p[i], p[next]);
    }
    return abs(doubleArea);
}

int interiorPoints(const vector<pt>& p) {
    return (doubleAreaPolygon(p) - boundaryPoints(p) + 2) / 2;
}

// =============================================================================
//                              ADVANCED POLYGONS
// =============================================================================

bool cw(pt a, pt b, pt c, bool include_collinear) {
    int o = sgn(orient(a, b, c));
    return o < 0 || (include_collinear && o == 0);
}

bool collinear(pt a, pt b, pt c) { return sgn(orient(a, b, c)) == 0; }

void convex_hull(vector<pt>& a, bool include_collinear = false) {
    if (a.empty()) return;
    pt p0 = *min_element(a.begin(), a.end(), [](pt a, pt b) {
        return make_pair(a.y, a.x) < make_pair(b.y, b.x);
    });
    sort(a.begin(), a.end(), [&p0](const pt& a, const pt& b) {
        int o = sgn(orient(p0, a, b));
        if (o == 0) return sq(p0 - a) < sq(p0 - b);
        return o < 0;
    });
    if (include_collinear) {
        int i = (int)a.size() - 1;
        while (i >= 0 && collinear(p0, a[i], a.back())) i--;
        reverse(a.begin() + i + 1, a.end());
    }
    vector<pt> st;
    for (int i = 0; i < (int)a.size(); i++) {
        while (st.size() > 1 && !cw(st[st.size() - 2], st.back(), a[i], include_collinear))
            st.pop_back();
        if (st.empty() || a[i] != st.back())
            st.push_back(a[i]);
    }
    if (!include_collinear && st.size() == 2 && st[0] == st[1])
        st.pop_back();
    a = st;
}

void reorder_polygon(vector<pt> & P) {
    size_t pos = 0;
    for (size_t i = 1; i < P.size(); i++) {
        if (P[i].y < P[pos].y || (P[i].y == P[pos].y && P[i].x < P[pos].x))
            pos = i;
    }
    rotate(P.begin(), P.begin() + pos, P.end());
}

// P and Q must be counter clockwise
vector<pt> minkowski(vector<pt> P, vector<pt> Q) {
    reorder_polygon(P);
    reorder_polygon(Q);
    P.push_back(P[0]); P.push_back(P[1]);
    Q.push_back(Q[0]); Q.push_back(Q[1]);
    
    vector<pt> result;
    size_t i = 0, j = 0;
    while (i < P.size() - 2 || j < Q.size() - 2) {
        result.push_back(P[i] + Q[j]);
        auto crs = cross(P[i + 1] - P[i], Q[j + 1] - Q[j]);
        if (crs >= 0 && i < P.size() - 2) ++i;
        if (crs <= 0 && j < Q.size() - 2) ++j;
    }
    return result;
}

double maximum_dist_from_polygon_to_polygon(vector<pt> &u, vector<pt> &v) {
    int n = (int)u.size(), m = (int)v.size();
    double ans = 0;
    if (n < 3 || m < 3) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) 
                ans = max(ans, (double)sq(u[i] - v[j]));
        return sqrt(ans);
    }
    if (u[0].x > v[0].x) { swap(n, m); swap(u, v); }
    
    int i = 0, j = 0, step = n + m + 10;
    while (j + 1 < m && v[j].x < v[j + 1].x) j++;
    while (step--) {
        if (cross(u[(i + 1) % n] - u[i], v[(j + 1) % m] - v[j]) >= 0) j = (j + 1) % m;
        else i = (i + 1) % n;
        ans = max(ans, (double)sq(u[i] - v[j]));
    }
    return sqrt(ans);
}

// =============================================================================
//                              HALF-PLANE INTERSECTION
// =============================================================================

struct Halfplane {
    pt p, pq;
    long double angle;

    Halfplane() {}
    Halfplane(const pt& a, const pt& b) : p(a), pq(b - a) {
        angle = atan2l(pq.y, pq.x);
    }

    bool out(const pt& r) {
        return cross(pq, r - p) < -EPS;
    }

    bool operator < (const Halfplane& e) const { return angle < e.angle; }

    friend pt inter(const Halfplane& s, const Halfplane& t) {
        long double alpha = cross((t.p - s.p), t.pq) / cross(s.pq, t.pq);
        return s.p + (s.pq * alpha);
    }
};

vector<pt> hp_intersect(vector<Halfplane>& H) {
    const int inf = 1e9;
    pt box[4] = { pt(inf, inf), pt(-inf, inf), pt(-inf, -inf), pt(inf, -inf) };
    for (int i = 0; i < 4; i++) {
        Halfplane aux(box[i], box[(i + 1) % 4]);
        H.push_back(aux);
    }

    sort(H.begin(), H.end());
    deque<Halfplane> dq;
    int len = 0;

    for (int i = 0; i < int(H.size()); i++) {
        while (len > 1 && H[i].out(inter(dq[len - 1], dq[len - 2]))) {
            dq.pop_back(); --len;
        }
        while (len > 1 && H[i].out(inter(dq[0], dq[1]))) {
            dq.pop_front(); --len;
        }

        if (len > 0 && fabsl(cross(H[i].pq, dq[len - 1].pq)) < EPS) {
            if (dot(H[i].pq, dq[len - 1].pq) < 0.0) return vector<pt>();
            if (H[i].out(dq[len - 1].p)) {
                dq.pop_back(); --len;
            } else continue;
        }
        dq.push_back(H[i]); ++len;
    }

    while (len > 2 && dq[0].out(inter(dq[len - 1], dq[len - 2]))) {
        dq.pop_back(); --len;
    }
    while (len > 2 && dq[len - 1].out(inter(dq[0], dq[1]))) {
        dq.pop_front(); --len;
    }

    if (len < 3) return vector<pt>();

    vector<pt> ret(len);
    for (int i = 0; i + 1 < len; i++) {
        ret[i] = inter(dq[i], dq[i + 1]);
    }
    ret.back() = inter(dq[len - 1], dq[0]);
    return ret;
}

// =============================================================================
//                              O(LOG N) CONVEX INSIDE
// =============================================================================

bool inConvex(vector<pt> &p, pt a) {
    if (p.size() < 3) return false;
    int n = p.size(), l = 1, r = n - 2, mid, ans = 1;
    while (l <= r) {
        mid = (l + r) / 2;
        if (sgn(orient(p[0], p[mid], a)) > 0) {
            l = mid + 1;
            ans = mid;
        } else {
            r = mid - 1;
        }
    }
    return inPolygon({p[0], p[ans], p[ans + 1]}, a, false);
}

bool StrictInConvex(vector<pt> &p, pt a) {
    if (p.size() < 3) return false;
    int n = p.size(), l = 1, r = n - 2, mid, ans = 1;
    while (l <= r) {
        mid = (l + r) / 2;
        if (sgn(orient(p[0], p[mid], a)) > 0) {
            l = mid + 1;
            ans = mid;
        } else {
            r = mid - 1;
        }
    }
    bool f = true;
    if (ans == 1) f = !onSegment(p[1], p[2], a) && !onSegment(p[0], p[1], a);
    else if (ans == n - 2) f = !onSegment(p[n - 2], p[n - 1], a) && !onSegment(p[n - 1], p[0], a);

    return inPolygon({p[0], p[ans], p[ans + 1]}, a, false) && f;
}

// =============================================================================
//                              ANTIPODAL POINTS
// =============================================================================

vector<pair<int, int>> all_anti_podal(int n, vector<pt> &p) {
    vector<pair<int, int>> result;
    auto nx = [&](int i) { return (i + 1) % n; };
    auto pv = [&](int i) { return (i - 1 + n) % n; };
    vector<bool> vis(n, false);

    for (int p1 = 0, p2 = 0; p1 < n; ++p1) {
        pt base = p[nx(p1)] - p[p1];
        while (p2 == p1 || p2 == nx(p1) || sgn(cross(base, p[nx(p2)] - p[p2])) == sgn(cross(base, p[p2] - p[pv(p2)]))) {
            p2 = nx(p2);
        }
        if (vis[p1]) continue;
        vis[p1] = true;
        result.push_back({p1, p2});
        result.push_back({nx(p1), p2});

        if (sgn(cross(base, p[nx(p2)] - p[p2])) == 0) {
            result.push_back({p1, nx(p2)});
            result.push_back({nx(p1), nx(p2)});
            vis[p2] = true;
        }
    }
    return result;
}
