#include "bits/stdc++.h"

using namespace std;

template<class T>
struct SparseTable {
    static const int B = 21;
    vector<array<T, B>> t;
    vector<int> lg;

    SparseTable(int n, vector<T> &v) : t(n + 1), lg(n + 1) {
        lg[n] = __lg(n);
        for (int i = 0; i < n; ++i) lg[i] = __lg(i), t[i][0] = v[i];
        for (int j = 1; j < B; ++j)
            for (int i = 0; i + (1 << j) - 1 < n; ++i)
                t[i][j] = merge(t[i][j - 1], t[i + (1 << (j - 1))][j - 1]);
    }

    T merge(T &l, T &r) { return min(l, r); }

    T query(int l, int r) {
        int j = lg[r - l + 1];
        return merge(t[l][j], t[r - (1 << j) + 1][j]);
    }
};

/*
 * One indexed
 * Add edges sorted by weight
 * Call Build after adding edges
 */
struct KRT {
    int n, nodes = 0;
    static const int B = 21;
    vector<vector<int>> adj;
    vector<array<int, 3>> edg;
    vector<array<int, 2>> tour;
    vector<array<array<int, 2>, B>> T;
    vector<int> p, d, in, lg;

    KRT(int n, int m) : n(n + m + 2), nodes(n), p(n + m + 2), adj(n + m + 2) {
        iota(p.begin(), p.end(), 0);
    }

    int find(int u) { return p[u] = u == p[u] ? u : find(p[u]); }

    void addEdge(int u, int v, int w = 0) {
        int pu = find(u), pv = find(v);
        if (pu == pv) return;
        edg.push_back({u, v, w});
        int id = nodes + size(edg);
        adj[id].push_back(pu);
        adj[id].push_back(pv);
        p[pu] = p[pv] = id;
    }

    void build() {
        d.resize(n), in.resize(n);
        for (int u = 1; u < n; ++u)
            if (u == p[u]) dfs(u);

        tour.reserve(n *= 2);
        T.resize(n), lg.resize(n);
        for (int i = 0; i < n; ++i) lg[i] = __lg(i), T[i][0] = tour[i];
        for (int j = 1; j < B; ++j)
            for (int i = 0; i + (1 << j) - 1 < n; ++i)
                T[i][j] = min(T[i][j - 1], T[i + (1 << (j - 1))][j - 1]);
    }

    void dfs(int u) {
        in[u] = size(tour);
        tour.push_back({d[u], u});
        for (int v: adj[u]) {
            d[v] = d[u] + 1, dfs(v);
            tour.push_back({d[u], u});
        }
    }

    int lca(int u, int v) {
        int l = min(in[u], in[v]), r = max(in[u], in[v]);
        int j = lg[r - l + 1];
        return min(T[l][j], T[r - (1 << j) + 1][j])[1];
    }

    int query(int u, int v) {
        int lc = lca(u, v);
        return lc > nodes ? edg[lc - nodes - 1][2] : 0;
    }
};

void solve() {
    int n, m, q;
    cin >> n >> m >> q;
    KRT T(n, m);
    for (int i = 1; i <= m; ++i) {
        int u, v;
        cin >> u >> v;
        T.addEdge(u, v, i);
    }

    T.build();
    vector<array<int, 2>> a(n + 1), b(n + 1);
    for (int u = 1; u <= n; ++u)
        a[u] = {T.in[u], u}, b[u] = {-T.in[u], u};

    SparseTable mn(n + 1, a), mx(n + 1, b);

    while (q--) {
        int l, r;
        cin >> l >> r;
        int u = mn.query(l, r)[1], v = mx.query(l, r)[1];
        cout << (u == v ? 0 : T.query(u, v)) << ' ';
    }

    cout << '\n';
}

signed main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);
#ifdef HALZOOM
    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif

    int t = 1;
    cin >> t;
    while (t--) solve();
}
