#include <bits/stdc++.h>

using namespace std;
#define int long long
const int inf = 1e18;

template<typename T>
struct BIT2D {
    int n;
    vector<vector<int>> vals;
    vector<vector<T>> bit;

    int ind(const vector<int> &v, int x) {
        return upper_bound(begin(v), end(v), x) - begin(v) - 1;
    }

    // n: the limit of the first dimension
    // todo: all update operations you will make
    BIT2D(int n, vector<array<int, 2>> &todo) : n(n + 1), vals(n + 1), bit(n + 1) {
        sort(begin(todo), end(todo), [](auto &a, auto &b) { return a[1] < b[1]; });

        for (int i = 0; i < n; i++) vals[i].push_back(-inf);
        for (auto [r, c]: todo)
            for (; r < n; r |= r + 1)
                if (vals[r].back() != c) vals[r].push_back(c);

        for (int i = 0; i < n; i++) bit[i].resize(vals[i].size());
    }

    void add(int r, int c, T val) {
        for (; r < n; r |= r + 1) {
            int i = ind(vals[r], c);
            for (; i < bit[r].size(); i |= i + 1) bit[r][i] = max(bit[r][i], val); // change to sum
        }
    }

    T query(int r, int c) {
        T ans = 0;
        for (; r >= 0; r = (r & r + 1) - 1) {
            int i = ind(vals[r], c);
            for (; i >= 0; i = (i & i + 1) - 1) ans = max(ans, bit[r][i]); // change to sum
        }
        return ans;
    }

    T query(int r1, int c1, int r2, int c2) {
        return query(r2, c2) - query(r2, c1 - 1) - query(r1 - 1, c2) + query(r1 - 1, c1 - 1);
    }

    void reset() {
        for (auto &v: bit) for (auto &i: v) i = 0;
    }
};

void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 1), b(n + 1);
    for (int i = 1, x; i <= n; ++i)
        cin >> x, a[x] = i;
    for (int i = 1, x; i <= n; ++i)
        cin >> x, b[x] = i;

    // LCIS
    vector<array<int, 2>> points;
    for (int i = 1; i <= n; i++)
        points.push_back({a[i], b[i]});
    BIT2D<int> bit(n + 1, points);

    for (int i = 1; i <= n; ++i) {
        int add = bit.query(a[i], b[i]) + 1;
        bit.add(a[i], b[i], add);
    }
    cout << bit.query(n, n) << '\n';
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef HALZOOM
    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif

    int test = 1;
    cin >> test;

    for (int i = 1; i <= test; ++i) {
        solve();
    }
    return 0;
}