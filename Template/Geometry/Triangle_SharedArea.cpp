#include<bits/stdc++.h>
using namespace std;
#define int long long

const double EPS = 1e-9;

struct Point {
    double x, y;

    bool operator==(const Point &p) const {
        return fabs(x - p.x) < EPS && fabs(y - p.y) < EPS;
    }
};

double cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }

Point operator-(Point a, Point b) { return {a.x - b.x, a.y - b.y}; }

// Shoelace formula for polygon area
double polygon_area(const vector <Point> &pts) {
    double area = 0;
    int n = pts.size();
    for (int i = 0; i < n; ++i) area += cross(pts[i], pts[(i + 1) % n]);
    return fabs(area) / 2;
}

// Check if point p is inside triangle abc
bool point_in_triangle(Point p, Point a, Point b, Point c) {
    double A = fabs(cross(b - a, c - a));
    double A1 = fabs(cross(a - p, b - p));
    double A2 = fabs(cross(b - p, c - p));
    double A3 = fabs(cross(c - p, a - p));
    return fabs(A - (A1 + A2 + A3)) < EPS;
}

// Segment-segment intersection
bool seg_intersect(Point a, Point b, Point c, Point d, Point &out) {
    Point r = b - a, s = d - c;
    double denom = cross(r, s);
    if (fabs(denom) < EPS) return false;  // Parallel or colinear
    double t = cross(c - a, s) / denom;
    double u = cross(c - a, r) / denom;
    if (t >= -EPS && t <= 1 + EPS && u >= -EPS && u <= 1 + EPS) {
        out = {a.x + t * r.x, a.y + t * r.y};
        return true;
    }
    return false;
}

// Remove duplicate points
void unique_points(vector <Point> &pts) {
    sort(pts.begin(), pts.end(), [](Point a, Point b) {
        return a.x < b.x - EPS || (fabs(a.x - b.x) < EPS && a.y < b.y - EPS);
    });
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
}

// Sort points around centroid (for convex polygon area)
void sort_ccw(vector <Point> &pts) {
    Point center{0, 0};
    for (auto &p: pts) {
        center.x += p.x;
        center.y += p.y;
    }
    center.x /= pts.size();
    center.y /= pts.size();
    sort(pts.begin(), pts.end(), [&](Point a, Point b) {
        double angleA = atan2(a.y - center.y, a.x - center.x);
        double angleB = atan2(b.y - center.y, b.x - center.x);
        return angleA < angleB;
    });
}

// Main function: compute shared area of two triangles
double shared_triangle_area(Point A, Point B, Point C, Point D, Point E, Point F) {
    vector <Point> poly;
    // Add all intersection points between triangle edges
    Point out;
    vector <pair<Point, Point>> edges1 = {{A, B},
                                          {B, C},
                                          {C, A}};
    vector <pair<Point, Point>> edges2 = {{D, E},
                                          {E, F},
                                          {F, D}};
    for (auto &[p1, p2]: edges1) {
        for (auto &[q1, q2]: edges2) {
            if (seg_intersect(p1, p2, q1, q2, out)) poly.push_back(out);
        }
    }
    // Add vertices of triangle 1 inside triangle 2
    for (Point p: {A, B, C})
        if (point_in_triangle(p, D, E, F)) poly.push_back(p);
    // Add vertices of triangle 2 inside triangle 1
    for (Point p: {D, E, F})
        if (point_in_triangle(p, A, B, C)) poly.push_back(p);
    unique_points(poly);
    if (poly.size() < 3) return 0.0;  // No overlapping area
    sort_ccw(poly);
    return polygon_area(poly);
}

signed main() {
    double a, b, c;
    cin >> a >> b >> c;
    Point A = {0, 0}, B = {0, c}, C = {c, 0};
    Point D = {0, 0}, E = {a, 0}, F = {0, b};
    double valid = shared_triangle_area(A, B, C, D, E, F);
    cout << valid << '\n';
}