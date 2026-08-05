#include <bits/stdc++.h>

using namespace std;

#define int long long
#define Om4rr cin.tie(nullptr); cout.tie(nullptr); ios::sync_with_stdio(false);

struct pt {
    int x, y;

    bool operator<(const pt &p) const {
        if (x != p.x) return x < p.x;
        return y < p.y;
    }
};

__int128 cross(pt a, pt b, pt c) {
    return (__int128) (b.x - a.x) * (c.y - a.y)
           - (__int128) (b.y - a.y) * (c.x - a.x);
}

int dist(pt a, pt b) {
    int x = a.x - b.x;
    int y = a.y - b.y;
    return x * x + y * y;
}

int isqrt(int x) {
    int r = sqrtl((long double) x);

    while ((__int128) (r + 1) * (r + 1) <= x)
        ++r;

    while ((__int128) r * r > x)
        --r;

    return r;
}

int closest_pair(const vector<pt> &p) {
    int n = p.size();
    int mn = LLONG_MAX, j = 0;

    set<pair<int, int>> st;

    for (int i = 0; i < n; ++i) {
        while (j < i and
               (__int128) (p[i].x - p[j].x) * (p[i].x - p[j].x) >= mn) {
            st.erase({p[j].y, p[j].x});
            ++j;
        }

        int d = isqrt(mn) + 1;

        auto l = st.lower_bound({p[i].y - d, LLONG_MIN});
        auto r = st.upper_bound({p[i].y + d, LLONG_MAX});

        for (auto it = l; it != r; ++it)
            mn = min(mn, dist(p[i], {it->second, it->first}));

        st.insert({p[i].y, p[i].x});
    }

    return mn;
}

vector<pt> convex_hull(const vector<pt> &p) {
    int n = p.size();

    if (n <= 2)
        return p;

    vector<pt> h(2 * n);
    int sz = 0;

    for (auto x: p) {
        while (sz >= 2 and cross(h[sz - 2], h[sz - 1], x) <= 0)
            --sz;

        h[sz++] = x;
    }

    int low = sz;

    for (int i = n - 2; i >= 0; --i) {
        while (sz > low and cross(h[sz - 2], h[sz - 1], p[i]) <= 0)
            --sz;

        h[sz++] = p[i];
    }

    h.resize(sz - 1);
    return h;
}

int farthest_pair(const vector<pt> &p) {
    int n = p.size();

    if (n == 2)
        return dist(p[0], p[1]);

    int mx = 0, j = 1;

    for (int i = 0; i < n; ++i) {
        int ni = (i + 1) % n;

        while (cross(p[i], p[ni], p[(j + 1) % n]) >
               cross(p[i], p[ni], p[j])) {
            j = (j + 1) % n;
        }

        mx = max(mx, dist(p[i], p[j]));
        mx = max(mx, dist(p[ni], p[j]));

        int nj = (j + 1) % n;

        if (cross(p[i], p[ni], p[nj]) ==
            cross(p[i], p[ni], p[j])) {
            mx = max(mx, dist(p[i], p[nj]));
            mx = max(mx, dist(p[ni], p[nj]));
        }
    }

    return mx;
}

void solve() {
    int n;
    cin >> n;

    vector<pt> p(n);

    for (auto &[x, y]: p)
        cin >> x >> y;

    sort(p.begin(), p.end());

    int mn = closest_pair(p);

    vector<pt> hull = convex_hull(p);
    int mx = farthest_pair(hull);

    unsigned long long ans = (unsigned long long) mn + (unsigned long long) mx;

    cout << ans << '\n';
}

signed main() {
    Om4rr

    int ts = 1;
    cin >> ts;

    while (ts--)
        solve();

    return 0;
}