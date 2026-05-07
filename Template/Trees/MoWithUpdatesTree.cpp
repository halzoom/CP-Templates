#include <bits/stdc++.h>

using namespace std;
#define int long long
const int inf = 1e18;
const int N = 2e5 + 5, LG = 25;
int in[N], out[N], tree[2 * N], depth[N], timer = 1;
vector<vector<int>> adj, ancestor;

int KthAnc(int u, int k) {
    for (int i = 0; k; ++i, k >>= 1)
        if (k & 1) u = ancestor[i][u];
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
    int l, r, t, id, lca;

    Query(int u, int v, int cnt, int i) {
        id = i, t = cnt;
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

};

struct Update {
    int pos, old, newv;
};

struct MoWithUpdates {
    int n, answer = 0, B;
    vector<int> a, res, cur, frq, vis;
    vector<Query> queries;
    vector<Update> updates;

    MoWithUpdates(vector<int> &_a) {
        n = _a.size();
        a = _a;
        B = pow(n, 2.0 / 3.0) + 1;
        frq.assign(N, {});
        vis.assign(N, {});
    }

    void addUpdate(int pos, int val) {
        updates.push_back({pos, 0, val});
    }

    void addQuery(int l, int r) {
        queries.emplace_back(l, r, updates.size(), queries.size());
    }

    void add(int x) {
        x = cur[x];
        if (++frq[x] == 1)answer++;
    }

    void erase(int x) {
        x = cur[x];
        if (--frq[x] == 0)answer--;
    }

    void go(int idx) {
        int u = tree[idx];
        vis[u] ^= 1;
        if (vis[u]) add(u);
        else erase(u);
    }

    int get() {
        return answer;
    }

    vector<int> Process() {
        sort(queries.begin(), queries.end(), [&](const Query &a, const Query &b) {
            int bl = a.l / B, bl2 = b.l / B;
            if (bl != bl2) return bl < bl2;
            int br = a.r / B, br2 = b.r / B;
            if (br != br2) return (bl & 1) ? br > br2 : br < br2;
            return (br & 1) ? a.t > b.t : a.t < b.t;
        });
        cur = a;
        for (auto &u: updates) {
            u.old = cur[u.pos];
            cur[u.pos] = u.newv;
        }
        res.assign(queries.size(), 0);
        cur = a;
        int l = 1, r = 0, t = 0;

        auto apply = [&](int idx) {
            int pos = updates[idx].pos;
            int newV = updates[idx].newv;
            if (vis[pos]) erase(pos);
            cur[pos] = newV;
            if (vis[pos]) add(pos);
        };

        auto cancel = [&](int idx) {
            int pos = updates[idx].pos;
            int oldV = updates[idx].old;
            if (vis[pos]) erase(pos);
            cur[pos] = oldV;
            if (vis[pos]) add(pos);
        };

        for (const auto &[lq, rq, tq, idx, lca]: queries) {
            while (t < tq) apply(t++);
            while (t > tq) cancel(--t);

            while (lq < l) --l, go(l);
            while (rq > r) ++r, go(r);
            while (lq > l) go(l), ++l;
            while (rq < r) go(r), --r;

            if (~lca) add(lca);
            res[idx] = get();
            if (~lca) erase(lca);
        }
        return res;
    }
};

void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n + 1);
    map<int, int> mp;
    for (int i = 1; i <= n; ++i)
        cin >> a[i], mp[a[i]];
    adj.assign(n + 1, {});
    ancestor.assign(LG, vector<int>(n + 1));
    for (int i = 1, u, v; i < n; ++i) {
        cin >> u >> v;
        adj[u].emplace_back(v);
        adj[v].emplace_back(u);
    }
    tour(1, 0);
    vector<array<int, 3>> Q(q);
    for (int i = 0; i < q; ++i) {
        cin >> Q[i][0] >> Q[i][1] >> Q[i][2];
        if (Q[i][0] == 2)
            mp[Q[i][2]];
    }
    int id = 1;
    for (auto &[_, v]: mp)
        v = id++;
    for (int i = 1; i <= n; ++i)
        a[i] = mp[a[i]];

    MoWithUpdates T(a);
    for (auto [t, l, r]: Q) {
        if (t == 1)
            T.addQuery(l, r);
        else
            T.addUpdate(l, mp[r]);
    }
    auto answer = T.Process();
    for (auto i: answer)
        cout << i << '\n';
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