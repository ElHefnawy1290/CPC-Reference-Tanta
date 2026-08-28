
// cosine rule:

c^2 = a^2 + b^2 - 2ab * cos(C) a^2 = b^2 + c^2 - 2bc * cos(A) b^2 = a^2 + c^2 - 2ac * cos(B)

cos(C) = (a^2 + b^2 - c^2) / (2ab) cos(A) = (b^2 + c^2 - a^2) / (2bc) cos(B) = (a^2 + c^2 - b^2) / (2ac)

C = cos^(-1) [(a^2 + b^2 - c^2) / (2ab)] A = cos^(-1) [(b^2 + c^2 - a^2) / (2bc)] B = cos^(-1) [(a^2 + c^2 - b^2) / (2ac)]


   // if I am given a set of points how to sort counter clock wise around the origin

// The arg() Function: This function automatically returns a mathematical value strictly within the
// interval $(-\pi, \pi]$, which perfectly aligns with the required counter-clockwise sweep.
    
bool comp(pt &a, pt &b) { 
    return arg(a) < arg(b); 
}

// cog
pt centerOfGravity(const vector<pt>& p) {
    int n = p.size();
    pt centroid = {0, 0};
    ld doubleArea = 0.0;

    for (int i = 0; i < n; i++) {
        int next = (i + 1) % n;
        ld c = cross(p[i], p[next]); 
        
        doubleArea += c;
        centroid += (p[i] + p[next]) * c; 
    }

    // Safety check to prevent division by zero for degenerate polygons
    if (fabs(doubleArea) < EPS) return centroid; 

    return centroid / (3.0 * doubleArea); 
}



typedef long double ld;
const ld eps = 1e-9;

struct P {
    ld x, y;

    // Constructor
    P(ld _x = 0, ld _y = 0) : x(_x), y(_y) {}

    // Overload subtraction to get a directional vector between two points
    P operator-(const P& other) const {
        return P(x - other.x, y - other.y);
    }

    // Overload ^ for 2D Cross Product
    ld operator^(const P& other) const {
        return x * other.y - y * other.x;
    }

    // Overload * for 2D Dot Product (Optional, but highly recommended)
    ld operator*(const P& other) const {
        return x * other.x + y * other.y;
    }

    // Overload < for sorting (sorts by X first, then by Y)
    // Used in Convex Hull and as a fallback in your polar sort
    bool operator<(const P& other) const {
        if (fabs(x - other.x) > eps) {
            return x < other.x;
        }
        return y < other.y;
    }

    // Overload == to check if two points are identical
    bool operator==(const P& other) const {
        return fabs(x - other.x) < eps && fabs(y - other.y) < eps;
    }
};

// Helper function used throughout your template to get a vector from A to B
inline P vec(P a, P b) {
    return b - a;
}




double get_full_angle(P a) {
    // atan2 takes (y, x), NOT (x, y)
    double ang = atan2(a.y, a.x);
    
    // If the angle is negative (quadrants 3 and 4), add 2*PI
    if (ang < 0) {
        ang += 2 * PI;
    }
    
    return ang;
}

// If you need the result in degrees from 0 to 360
double get_full_angle_degrees(P a) {
    double ang = atan2(a.y, a.x);
    if (ang < 0) {
        ang += 2 * PI;
    }
    return ang * (180.0 / PI);
}

// check if a,b,c are collinear
bool collinear(P a, P b, P c)  
{  
    return (vec(a, b) ^ vec(a, c)) == 0;  
}

// to sort points in counterclockwise order around a point
struct cmp  
{  
    P about;  
    cmp(P c)  
    {  
       about = c;  
    }  
    bool operator()(const P& a, const P& b) const  
    {  
       ld cr = vec(about, a) ^ vec(about, b);  
       if (fabs(cr) < eps)  
          return a < b;  
       return cr > 0;  
    }  
};  

void sortAntiClockWise(vector<P>& pnts)  
{  
    P mn(*min_element(all(pnts)));  
    sort(pnts.begin(), pnts.end(), cmp(mn));  
}  
inline bool pibb(P const& a, P const& b1, P const& b2)  
{  
    return a.x >= min(b1.x, b2.x) &&  
       a.x <= max(b1.x, b2.x) &&  
       a.y >= min(b1.y, b2.y) &&  
       a.y <= max(b1.y, b2.y);  
} 


// Returns the center of the circle passing through points A, B, and C
P getCircleCenter(P A, P B, P C) {  
    if (isCollinear(A, B, C)) {  
       collinear = true;  
       return {0, 0};   
    }  
    collinear = false;  
    ld D = 2 * (A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));  
    ld Ux = ((A.x * A.x + A.y * A.y) * (B.y - C.y) + (B.x * B.x + B.y * B.y) * (C.y - A.y) + (C.x * C.x + C.y * C.y) * (A.y - B.y)) / D;  
    ld Uy = ((A.x * A.x + A.y * A.y) * (C.x - B.x) + (B.x * B.x + B.y * B.y) * (A.x - C.x) + (C.x * C.x + C.y * C.y) * (B.x - A.x)) / D;  
    return {Ux, Uy};  
}



// compute the convex hull of a set of points using Andrew's monotone chain algorithm
// hull will contain the points of the convex hull in counterclockwise order

void convexHull(vector<P> p, vector<P>& hull)  
{  
  
    sort(all(p), [&](P& a, P& b)  
    {  
      if (a.x != b.x)  
         return a.x < b.x;  
      return a.y < b.y;  
    });  
    if (p.size() == 1)  
    {  
       hull.push_back(p[0]);  
       return;  
    }  
    for (int rep = 0; rep < 2; rep++)  
    {  
       int s = hull.size();  
       for (int i = 0; i < p.size(); i++)  
       {  
          while (hull.size() >= s + 2)  
          {  
             P p1 = hull.end()[-2];  
             P p2 = hull.end()[-1];  
             if ((vec(p1, p2) ^ vec(p1, p[i])) < -eps)  
                break;  
  
             hull.pop_back();  
          }  
          hull.push_back(p[i]);  
       }  
       reverse(all(p));  
       hull.pop_back();  
    }  
}


// Returns the centroid of a polygon defined by the points in vector p
// centroid is the center of mass of the polygon, assuming uniform density

P getCentroid(vector<P>& p)  
{  
    ld x, y;  
    long long tarea = 0;  
    x = 0;  
    y = 0;  
    for (int i = 1; i < p.size() - 2; i++)  
    {  
       long long area = (vec(p[0], p[i]) ^ vec(p[0], p[i + 1]));  
       x += area * ((p[0].x + p[i].x + p[i + 1].x) / 3.0);  
       y += area * ((p[0].y + p[i].y + p[i + 1].y) / 3.0);  
       tarea += area;  
    }  
    x /= tarea;  
    y /= tarea;  
    return { x, y };  
}


// some properties of regular polygons are listed below.

Properties of Regular polygons Some of the properties of regular polygons are listed below.

All the sides of a regular polygon are equal
All the interior angles are equal 
The perimeter of a regular polygon with n sides is equal to the n times of a side measure. 
The sum of all the interior angles of a simple n-gon or regular polygon = (n − 2) × 180° 
The number of diagonals in a polygon with n sides = n(n – 3)/2 
The number of triangles formed by joining the diagonals from one corner of a polygon = n – 2 
The measure of each interior angle of n-sided regular polygon = [(n – 2) × 180°]/n 
The measure of each exterior angle of an n-sided regular polygon = 360°/n

area of reqular polygon = ((l^2)*n)/(4tan(PI/n)) l is the side length

n is the number of sides
