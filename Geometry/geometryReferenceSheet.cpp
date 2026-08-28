// =============================================================================
//                              BASICS & TYPES
// =============================================================================
int sgn(T val);                  // Returns -1 (negative), 1 (positive), or 0.
T sq(pt p);                      // Squared length of vector p.
ld abs(pt p);                    // Length of vector p.
pt perp(pt p);                   // Returns vector rotated 90 degrees CCW.
T dot(pt v, pt w);               // Dot product.
T cross(pt v, pt w);             // Cross product (v X w).
bool isPerp(pt v, pt w);         // True if dot product is 0.
pt unitVector(pt p);             // Vector of length 1 in the same direction.

// =============================================================================
//                              TRANSFORMATIONS
// =============================================================================
pt translate(pt v, pt p);        // Moves point p by vector v.
pt scale(pt c, T factor, pt p);  // Scales p away from center c by factor.
pt rot(pt p, pt c, ld a);        // Rotates p around c by angle a (in radians).
pt linearTransfo(pt p, pt q, pt r, pt fp, pt fq); // Maps r given that p->fp and q->fq.

// =============================================================================
//                              ANGLES
// =============================================================================
T orient(pt a, pt b, pt c);      // Relative to AB: > 0 (C is left), < 0 (C is right), 0 (collinear).
bool inAngle(pt a, pt b, pt c, pt p); // True if p is inside CCW angle BAC.
T angle(pt v, pt w);             // Shortest positive non-oriented angle [0, PI].
ld orientedAngle(pt a, pt b, pt c); // CCW angle BAC in range [0, 2*PI).
ld angleTravelled(pt a, pt p, pt q); // Signed angle travelled from P to Q around A.

// =============================================================================
//                              LINES (struct line)
// =============================================================================
line(pt v, T c);                 // From direction vector and offset.
line(T a, T b, T _c);            // From equation ax + by = c.
line(pt p, pt q);                // From two points P and Q.
T side(pt p);                    // > 0 (left of line), < 0 (right of line), 0 (on line).
double dist(pt p);               // Shortest distance from p to line.
line perpThrough(pt p);          // Returns perpendicular line passing through p.
line translate(pt t);            // Translates line by vector t.
line shiftLeft(double dist);     // Shifts line to the left by given distance.
pt proj(pt p);                   // Projection of point p onto the line.
pt refl(pt p);                   // Reflection of point p across the line.

bool inter(line l1, line l2, pt &out); // Line-line intersection. Returns false if parallel.
line bisector(line l1, line l2, bool interior); // Angle bisector of l1 and l2.

// =============================================================================
//                              SEGMENTS & RAYS
// =============================================================================
bool onSegment(pt a, pt b, pt p); // True if p lies on segment AB.
bool properInter(pt a, pt b, pt c, pt d, pt &out); // True if segments AB and CD intersect at exactly one interior point.
set<pair<ld, ld>> inters(pt a, pt b, pt c, pt d); // Returns all intersection points of segments AB and CD.
ld segPoint(pt a, pt b, pt p);    // Shortest distance from p to segment AB.
ld rayPoint(pt a, pt b, pt p);    // Shortest distance from p to ray AB.
ld segSeg(pt a, pt b, pt c, pt d); // Shortest distance between segment AB and segment CD.

// =============================================================================
//                              CIRCLES
// =============================================================================
pair<pt, T> circumCircle(pt a, pt b, pt c); // Returns {center, radius} of triangle ABC.
int circleLine(pt o, double r, line l, pair<pt, pt> &out); // Intersections of line and circle. Returns count (0, 1, or 2).
int circleCircle(pt o1, T r1, pt o2, T r2, pair<pt, pt> &out); // Intersections of 2 circles. Returns count.
int tangents(pt o1, T r1, pt o2, T r2, bool inner, vector<pair<pt, pt>> &out); // Tangent lines between 2 circles. 

// =============================================================================
//                              BASIC POLYGONS
// =============================================================================
ld areaPolygon(vector<pt> p);       // Area of any simple polygon.
bool isConvex(vector<pt> p);        // True if polygon is strictly convex.
bool inPolygon(vector<pt> p, pt a, bool strict = true); // O(N). True if a is inside. strict=true excludes boundaries.
pt polygonCentroid(const vector<pt>& p); // Center of gravity. Vertices must be ordered (CW/CCW).

// =============================================================================
//                              PICK'S THEOREM (Integer Coordinates)
// =============================================================================
int boundaryPoints(const vector<pt>& p);   // Number of integer points on boundary.
int doubleAreaPolygon(const vector<pt>& p); // 2 * Area (exact integer).
int interiorPoints(const vector<pt>& p);   // Number of strictly interior integer points.

// =============================================================================
//                              ADVANCED POLYGONS
// =============================================================================
void convex_hull(vector<pt>& a, bool include_collinear = false); // O(N log N). Modifies vector in-place.
vector<pt> minkowski(vector<pt> P, vector<pt> Q); // O(N+M). Minkowski sum. P and Q must be CCW convex.
double maximum_dist_from_polygon_to_polygon(vector<pt> &u, vector<pt> &v); // O(N+M). Max distance between two convex polygons.

// =============================================================================
//                              HALF-PLANE INTERSECTION
// =============================================================================
Halfplane(const pt& a, const pt& b); // Halfplane to the LEFT of directed line AB.
vector<pt> hp_intersect(vector<Halfplane>& H); // O(N log N). Returns CCW convex polygon of the intersection. Empty vector if no intersection.

// =============================================================================
//                              O(LOG N) CONVEX INSIDE
// =============================================================================
bool inConvex(vector<pt> &p, pt a); // O(log N). True if point inside convex polygon (including boundary).
bool StrictInConvex(vector<pt> &p, pt a); // O(log N). True if point strictly inside convex polygon.

// =============================================================================
//                              ANTIPODAL POINTS
// =============================================================================
vector<pair<int, int>> all_anti_podal(int n, vector<pt> &p); // O(N). Returns pairs of indices of antipodal points in a convex polygon.
