#include <bits/stdc++.h>

using namespace std;
#define int long long

struct HLD {
    vector<vector<int>> adj;
    vector<int> in, parent, head, depth, sz, big;
    int timer = 1;

    HLD(int n, vector<vector<int>> &_adj) : adj(_adj), parent(n + 1), in(n + 1), big(n + 1),
                                            head(n + 1), depth(n + 1), sz(n + 1) {
        go(1, 0);
        hld(1, 1);
    }

    void go(int u, int p) {
        sz[u] = 1;
        int bgChild = 0;
        for (auto v: adj[u]) {
            if (v == p)continue;
            depth[v] = depth[u] + 1;
            parent[v] = u;
            go(v, u);
            if (sz[v] > bgChild)big[u] = v, bgChild = sz[v];
            sz[u] += sz[v];
        }
    }

    void hld(int u, int id) {
        in[u] = timer++;
        head[u] = id;
        if (big[u])
            hld(big[u], id);
        for (auto v: adj[u]) {
            if (v == parent[u] or v == big[u])continue;
            hld(v, v);
        }
    }

    vector<pair<int, int>> path(int u, int v, bool onEdges = false) {
        vector<pair<int, int>> left, right;
        while (head[u] != head[v]) {
            if (depth[head[u]] >= depth[head[v]]) {
                left.emplace_back(in[head[u]], in[u]);
                u = parent[head[u]];
            } else {
                right.emplace_back(in[head[v]], in[v]);
                v = parent[head[v]];
            }
        }
        if (in[u] >= in[v])
            left.emplace_back(in[v], in[u]);
        else
            right.emplace_back(in[u], in[v]);
        reverse(right.begin(), right.end());
        for (auto x: right)
            left.emplace_back(x);
        return left;
    }
};

struct SegmentTree {
#define LNodeIDX (2 * node + 1)
#define RNodeIDX (2 * node + 2)
#define mid      ((l + r) / 2 )
    vector<int> Tree;
    int SegSize = 1;
    int defVal = 0;

    int Merge(int &a, int &b) {
        return max(a, b);
    }

    void update(int l, int r, int node, int idx, int val) {
        if (l == r) {
            Tree[node] = val;
            return;
        }

        if (idx <= mid) {
            update(l, mid, LNodeIDX, idx, val);
        } else {
            update(mid + 1, r, RNodeIDX, idx, val);
        }

        Tree[node] = Merge(Tree[LNodeIDX], Tree[RNodeIDX]);
    }

    int query(int l, int r, int node, int Lx, int Rx) {
        if (Lx > r or Rx < l) {
            return defVal;
        }
        if (Lx <= l and r <= Rx) {
            return Tree[node];
        }

        int L = query(l, mid, LNodeIDX, Lx, Rx);
        int R = query(mid + 1, r, RNodeIDX, Lx, Rx);

        return Merge(L, R);
    }

public:
    SegmentTree(int n) {
        while (SegSize < n)SegSize *= 2;
        Tree.assign(2 * SegSize, defVal);
    }

    void update(int idx, int val) {
        update(0, SegSize - 1, 0, idx, val);
    }

    int query(int l, int r) {
        return query(0, SegSize - 1, 0, l, r);
    }
};

void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n + 1);
    vector<vector<int>> adj(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> a[i];
    for (int i = 1, u, v; i < n; ++i) {
        cin >> u >> v;
        adj[u].emplace_back(v);
        adj[v].emplace_back(u);
    }
    HLD T(n, adj);
    SegmentTree seg(n + 1);
    for (int i = 1; i <= n; ++i)
        seg.update(T.in[i], a[i]);

    auto go = [&](int u, int v) {
        auto path = T.path(u, v);
        int answer = 0;
        for (auto [l, r]: path)
            answer = max(answer, seg.query(l, r));
        return answer;
    };
    while (q--) {
        int t, u, v;
        cin >> t >> u >> v;
        if (t == 1)
            seg.update(T.in[u], v);
        else
            cout << go(u, v) << ' ';
    }
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