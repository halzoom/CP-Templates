#include <bits/stdc++.h>

using namespace std;
#define int long long
const int N = 1e5 + 5, inf = 1e18;
int s[N], v[N], dp[N];
vector<vector<pair<int, int>>> adj;

template<bool IS_MAX = false, bool IS_INC = false>
struct RollbackCHT {
    struct Line {
        int m, c;

        Line() : m(0), c(inf) {}

        Line(int m, int c) : m(m), c(c) {}

        int eval(int x) const { return m * x + c; }
    };

    struct History {
        int pos;
        int sz;
        Line old_line;
    };

    vector<Line> st;
    vector<History> hist;
    int sz;

    RollbackCHT(int max_nodes) {
        st.resize(max_nodes);
        sz = 0;
    }

    bool redundant(Line l1, Line l2, Line l3) {
        __int128 dx1 = l1.m - l2.m;
        __int128 dc1 = l2.c - l1.c;
        __int128 dx2 = l2.m - l3.m;
        __int128 dc2 = l3.c - l2.c;
        return dc1 * dx2 >= dc2 * dx1;
    }

    void add(int m, int c) {
        int m_int = IS_INC ? -m : m;
        int c_int = IS_MAX ? -c : c;

        Line l_new(m_int, c_int);
        int pos = sz;
        int low = 1, high = sz - 1;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (redundant(st[mid - 1], st[mid], l_new)) {
                pos = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        hist.push_back({pos, sz, st[pos]});
        st[pos] = l_new;
        sz = pos + 1;
    }

    void rollback() {
        if (hist.empty()) return;
        History h = hist.back();
        hist.pop_back();
        sz = h.sz;
        st[h.pos] = h.old_line;
    }

    int query(int x) {
        if (sz == 0) return IS_MAX ? -inf : inf;

        int x_int = (IS_INC != IS_MAX) ? -x : x;

        int low = 0, high = sz - 2;
        int best = sz - 1;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (st[mid].eval(x_int) <= st[mid + 1].eval(x_int)) {
                best = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        int ans = st[best].eval(x_int);
        return IS_MAX ? -ans : ans;
    }
};

RollbackCHT<false, false> T(N);

void go(int u, int p, int d) {
    if (p)
        dp[u] = d * v[u] + s[u] + T.query(v[u]);

    T.add(-d, dp[u]);
    for (auto [x, add]: adj[u]) {
        if (x == p)continue;
        go(x, u, d + add);
    }

    T.rollback();
}

void solve() {
    int n;
    cin >> n;
    adj.assign(n + 1, {});
    for (int i = 1, u, v, w; i < n; ++i) {
        cin >> u >> v >> w;
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);
    }
    for (int i = 2; i <= n; ++i)
        cin >> s[i] >> v[i];


    go(1, 0, 0);
    for (int i = 2; i <= n; ++i)
        cout << dp[i] << ' ';
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