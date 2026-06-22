#include <bits/stdc++.h>

using namespace std;
#define int long long
const int mod = 1e9 + 7;

// Query -> O(1)
// Build  -> nlog(n)
template<typename T, class F>
struct SqrtTree {
    int n, levels;
    vector<vector<T>> st;
    F op;

    SqrtTree(const vector<T> &a, F _op) : n(a.size()), op(_op) {
        levels = n <= 1 ? 1 : 32 - __builtin_clz(n - 1);
        st.assign(levels, vector<T>(n));
        for (int i = 0; i < n; ++i) st[0][i] = a[i];

        for (int level = 1; level < levels; ++level) {
            int half = 1 << level;
            for (int i = half; i < n; i += half << 1) {
                st[level][i - 1] = a[i - 1];
                for (int j = i - 2; j >= i - half; --j)
                    st[level][j] = op(a[j], st[level][j + 1]);

                if (i < n) st[level][i] = a[i];
                for (int j = i + 1; j < min(n, i + half); ++j)
                    st[level][j] = op(st[level][j - 1], a[j]);
            }
        }
    }

    T query(int l, int r) {
        if (l == r) return st[0][l];
        int level = 31 - __builtin_clz(l ^ r);
        return op(st[level][l], st[level][r]);
    }
};

void solve() {
    int n;
    cin >> n;
    vector<pair<int, int>> a(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> a[i].first, a[i].second = 1;

    SqrtTree T(a, [&](pair<int, int> x, pair<int, int> y) {
        pair<int, int> c{};
        c.first = max(x.first, y.first);
        if (x.first > y.first)
            c.second = x.second;
        else if (x.first < y.first)
            c.second = y.second;
        else
            c.second = x.second + y.second;
        return c;
    });

    int q, x, y, z, t;
    cin >> q >> x >> y >> z >> t;
    int answer = 0, tot = 1;
    for (int i = 0; i < q; ++i) {
        int l = (answer * x + y) % n + 1;
        int r = (answer * z + t) % n + 1;
        if (l > r)swap(l, r);
        answer = T.query(l, r).second;
        tot = tot * answer % mod;
    }
    cout << tot << '\n';
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