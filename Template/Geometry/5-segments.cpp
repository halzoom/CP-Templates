#include<bits/stdc++.h>
using namespace std;
#define int long long

bool inDisk(pt a, pt b, pt p) { return dot(a - p, b - p) <= EPS; }

bool onSegment(pt a, pt b, pt p) {
    return fabsl(orient(a, b, p)) <= EPS && inDisk(a, b, p);
}

bool properInter(pt a, pt b, pt c, pt d, pt &out) {
    T oa = orient(c, d, a), ob = orient(c, d, b), oc = orient(a, b, c),
            od = orient(a, b, d);
    // Proper intersection exists iff opposite signs
    if (sgn(oa) * sgn(ob) < 0 && sgn(oc) * sgn(od) < 0) {
        out = (a * ob - b * oa) / (ob - oa);
        return true;
    }
    return false;
}

set <pair<ld, ld>> inters(pt a, pt b, pt c, pt d) {
    set <pair<ld, ld>> s;
    pt out;
    if (a == c || a == d) { s.insert(make_pair(a.x, a.y)); }
    if (b == c || b == d) { s.insert(make_pair(b.x, b.y)); }
    if (s.size()) return s;
    if (properInter(a, b, c, d, out)) return {make_pair(out.x, out.y)};
    if (onSegment(c, d, a)) s.insert(make_pair(a.x, a.y));
    if (onSegment(c, d, b)) s.insert(make_pair(b.x, b.y));
    if (onSegment(a, b, c)) s.insert(make_pair(c.x, c.y));
    if (onSegment(a, b, d)) s.insert(make_pair(d.x, d.y));
    return s;
}

ld segPoint(pt a, pt b, pt p) {
    if (a != b) {
        line l(a, b);
        if (l.cmpProj(a, p) && l.cmpProj(p, b))  // if closest to projection
            return l.dist(p);                      // output distance to line
    }
    return min(abs(p - a), abs(p - b));  // otherwise distance to A or B
}

ld segSeg(pt a, pt b, pt c, pt d) {
    pt dummy;
    if (properInter(a, b, c, d, dummy)) return 0;
    return min(
            {segPoint(a, b, c), segPoint(a, b, d), segPoint(c, d, a), segPoint(c, d, b)});
}
