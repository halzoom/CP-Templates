// check if there is a path from a to b without passing node c
#include <bits/stdc++.h>

using namespace std;
#define int long long

struct ArticulationPoints {
    int n, timer, sz;
    vector<vector<int>> adj, tree, BCC;
    vector<int> in, low, id, st, isArt;

    ArticulationPoints(int _n, const vector<vector<int>> &_adj) {
        n = _n;
        adj = _adj;
        build();
    }

    void build() {
        in.assign(n + 1, {});
        low.assign(n + 1, {});
        id.assign(n + 1, {});
        isArt.assign(n + 1, {});
        timer = 1;
        for (int i = 1; i <= n; i++)
            if (!in[i]) dfs(i, -1);

        sz = BCC.size();
        for (int i = 1; i <= n; i++)
            if (isArt[i]) id[i] = ++sz;

        tree.assign(sz + 1, {});
        for (int i = 0; i < BCC.size(); i++) {
            int IDs = i + 1;
            for (int u: BCC[i]) {
                if (isArt[u]) {
                    tree[IDs].push_back(id[u]);
                    tree[id[u]].push_back(IDs);
                } else
                    id[u] = IDs;
            }
        }
    }

    void dfs(int u, int p) {
        in[u] = low[u] = ++timer;
        st.emplace_back(u);
        int children = 0;

        for (int v: adj[u]) {
            if (v == p) continue;
            if (in[v]) {
                low[u] = min(low[u], in[v]);
            } else {
                children++;
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] >= in[u]) {
                    isArt[u] = (p != -1 or children > 1);
                    vector<int> component;
                    while (true) {
                        int node = st.back();
                        st.pop_back();
                        component.push_back(node);
                        if (node == v) break;
                    }
                    component.push_back(u);
                    BCC.push_back(component);
                }
            }
        }
    }
};

struct LCA {
    vector<vector<int>> ancestor;
    vector<int> level;
    int LG;

    LCA(vector<vector<int>> &adj) {
        int n = adj.size();
        LG = __lg(n) + 1;
        ancestor.assign(LG, vector<int>(n));
        level.assign(n, {});
        build(1, 0, adj);
        for (int i = 1; i < LG; ++i) {
            for (int u = 1; u < n; ++u) {
                ancestor[i][u] = ancestor[i - 1][ancestor[i - 1][u]];
            }
        }
    }

    void build(int u, int p, vector<vector<int>> &adj) {
        for (auto v: adj[u]) {
            if (v == p) continue;
            level[v] = level[u] + 1;
            ancestor[0][v] = u;
            build(v, u, adj);
        }
    }

    int KthAnc(int u, int k) {
        for (int i = 0; k; ++i, k >>= 1) {
            if (k & 1) u = ancestor[i][u];
        }
        return u;
    }

    int getLCA(int u, int v) {
        if (level[u] > level[v]) swap(u, v);
        int k = level[v] - level[u];
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

    int getDist(int u, int v) {
        int lca = getLCA(u, v);
        return level[u] + level[v] - 2 * level[lca];
    }

    bool is_on_path(int a, int b, int c) {
        int dab = getDist(a, b);
        int dac = getDist(a, c);
        int dcb = getDist(c, b);
        return dac + dcb == dab;
    }
};

void solve() {
    int n, m, q;
    cin >> n >> m >> q;
    vector<vector<int>> adj(n + 1);
    for (int i = 0, u, v; i < m; ++i) {
        cin >> u >> v;
        adj[u].emplace_back(v);
        adj[v].emplace_back(u);
    }

    ArticulationPoints T(n, adj);
    LCA lc(T.tree);

    while (q--) {
        int a, b, c;
        cin >> a >> b >> c;
        if (a == c or b == c) {
            cout << "NO\n";
            continue;
        }
        if (T.isArt[c] == 0) {
            cout << "YES\n";
            continue;
        }
        a = T.id[a], b = T.id[b], c = T.id[c];
        cout << (lc.is_on_path(a, b, c) ? "NO\n" : "YES\n");
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