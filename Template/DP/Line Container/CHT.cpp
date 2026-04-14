#include <bits/stdc++.h>

using namespace std;
#define int long long
const int inf = 1e18;

struct Line {
    mutable int m, c, p;

    bool operator<(const Line &o) const { return m < o.m; }

    bool operator<(int x) const { return p < x; }
};

struct HullDynamic : multiset<Line, less<>> {
    // (for doubles, use inf = 1/.0, div(a,b) = a/b)
    const int inf = 2e18;

    int div(int a, int b) { // floored division
        return a / b - ((a ^ b) < 0 && a % b);
    }

    bool isect(iterator x, iterator y) {
        if (y == end()) {
            x->p = inf;
            return false;
        }
        if (x->m == y->m)
            x->p = x->c > y->c ? inf : -inf;
        else
            x->p = div(y->c - x->c, x->m - y->m);
        return x->p >= y->p;
    }

    void insert_line(int m, int c) {
//        m *= -1, c *= -1; // for minimum
        auto z = insert({m, c, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z);
        if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p)
            isect(x, erase(y));
    }

    int get(int x) {
        assert(!empty());
        auto l = *lower_bound(x);
//        return -(l.m * x + l.c); // for minimum
        return l.m * x + l.c;
    }
};


void solve() {
    int n;
    cin >> n;
    vector<array<int, 3>> a(n);
    for (int i = 0; i < n; ++i)
        cin >> a[i][0] >> a[i][1] >> a[i][2];
    sort(a.begin(), a.end());

    HullDynamic T;
    T.insert_line(0, 0);
    vector<int> dp(n);
    for (int i = 0; i < n; ++i) {
        auto [r, c, ai] = a[i];
        dp[i] = T.get(c) + r * c - ai;
        T.insert_line(-r, dp[i]);
    }
    cout << *max_element(dp.begin(), dp.end()) << '\n';
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef HALZOOM
    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif

    int test = 1;
//    cin >> test;

    for (int i = 1; i <= test; ++i) {
        solve();
    }
    return 0;
}