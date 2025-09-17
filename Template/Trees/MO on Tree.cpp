#include<bits/stdc++.h>
using namespace std;
#define int long long

// count number of distinct colors in path
const int N = 2e5 + 5, B = 360, LG = 25;
int in[N], out[N], tree[2 * N], depth[N],
        timer = 1;  // don't forget to reset timer in test cases
vector <vector<int>> adj, ancestor;

int KthAnc(int u, int k) {
    for (int i = 0; k; ++i, k >>= 1) {
        if (k & 1) u = ancestor[i][u];
    }
    return u;
}

int LCA(int u, int v) {
    if (depth[u] > depth[v]) swap(u, v);
    int k = depth[v] - depth[u];
    v = KthAnc(v, k);
    if (v == u) return v;
    for (int i = LG - 1; ~i; --i) {
        if (ancestor[i][v] != ancestor[i][u]) {
            v = ancestor[i][v];
            u = ancestor[i][u];
        }
    }
    return ancestor[0][u];
}

void tour(int u, int p) {
    tree[timer] = u;
    in[u] = timer++;
    for (int i = 1; i < LG; ++i) { ancestor[i][u] = ancestor[i - 1][ancestor[i - 1][u]]; }
    for (auto v: adj[u]) {
        if (v == p) continue;
        depth[v] = depth[u] + 1;
        ancestor[0][v] = u;
        tour(v, u);
    }
    tree[timer] = u;
    out[u] = timer++;
}

struct Query {
    int l, r, lca, idx;

    Query(int u, int v, int i) {
        idx = i;
        if (in[u] > in[v]) swap(u, v);
        int lc = LCA(u, v);
        if (lc == u) {
            l = in[u], r = in[v];
            lca = -1;
        } else {
            l = out[u], r = in[v];
            lca = lc;
        }
    }

    inline pair<int, int> toPair() const {
        return make_pair(l / B, ((l / B) & 1) ? -r : +r);
    }
};

struct MO_onTree {
    vector<int> v, frq, vis;
    int n, ans = 0;

    MO_onTree(vector<int> &a) {
        v = a;
        n = (int) a.size();
        frq.assign(n + 1, 0);
        vis.assign(n + 1, 0);
    }

    void add(int u) {
        u = v[u];
        if (++frq[u] == 1) ans++;
    }

    void erase(int u) {
        u = v[u];
        if (--frq[u] == 0) ans--;
    }

    void go(int idx) {
        int u = tree[idx];
        vis[u] ^= 1;
        if (vis[u]) add(u);
        else erase(u);
    }

    vector<int> Process(vector <Query> &query) {
        sort(query.begin(), query.end(),
             [&](Query &a, Query &b) { return a.toPair() < b.toPair(); });
        vector<int> ret(query.size());
        int l = query[0].l, r = l;
        go(l);
        for (const auto &[lq, rq, lca, idx]: query) {
            while (lq < l) --l, go(l);
            while (rq > r) ++r, go(r);
            while (lq > l) go(l), ++l;
            while (rq < r) go(r), --r;
            if (~lca) add(lca);
            ret[idx] = ans;
            if (~lca) erase(lca);
        }
        return ret;
    }
};

void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> c(n + 1);
    adj.assign(n + 1, {});
    ancestor.assign(LG, vector<int>(n + 1));
    map<int, int> mp;
    for (int i = 1; i <= n; ++i) cin >> c[i], mp[c[i]];
    int id = 1;
    for (auto &[_, v]: mp) v = id++;
    for (int i = 1; i <= n; ++i) c[i] = mp[c[i]];
    for (int i = 1, a, b; i < n; ++i) {
        cin >> a >> b;
        adj[a].emplace_back(b);
        adj[b].emplace_back(a);
    }
    timer = 1;
    tour(1, 0);
    vector <Query> query;
    for (int i = 0, a, b; i < q; ++i) {
        cin >> a >> b;
        query.emplace_back(a, b, i);
    }
    MO_onTree mo(c);
    auto res = mo.Process(query);
    for (auto i: res) cout << i << '\n';
}