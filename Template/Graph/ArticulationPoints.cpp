// check if there is a path from a to b without passing node c
#include <bits/stdc++.h>

using namespace std;
#define int long long

struct Articulation {
    vector<vector<int>> adj, tree, Components;
    vector<int> low, vis, depth, is_art, id, done;
    stack<pair<int, int>> st;
    int n;

    Articulation(int n, vector<vector<int>> &adj) :
            adj(adj), n(n),
            low(n + 1, 0), vis(n + 1, 0), depth(n + 1, 0),
            is_art(n + 1, 0), id(n + 1, 0), done(n + 1, 0) {
        build();
    }

    void saturate(int u = -1, int v = -1) {
        vector<int> comp;
        while (!st.empty()) {
            auto [x, y] = st.top();
            st.pop();
            if (!done[x]) done[x] = 1, comp.emplace_back(x);
            if (!done[y]) done[y] = 1, comp.emplace_back(y);
            if (x == u and y == v) break;
        }
        for (int x: comp) done[x] = 0;
        Components.emplace_back(comp);
    }

    void dfs(int u, int p) {
        vis[u] = 1;
        low[u] = depth[u];
        int children = 0;

        for (auto v: adj[u]) {
            if (v == p) continue;
            if (!vis[v]) {
                st.emplace(u, v);
                children++;
                depth[v] = depth[u] + 1;
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                if ((p and low[v] >= depth[u]) || (!p and children > 1))
                    is_art[u] = 1;

                if (low[v] >= depth[u])saturate(u, v);
            } else if (vis[v] and depth[v] < depth[u]) {
                st.emplace(u, v);
                low[u] = min(low[u], depth[v]);
            }
        }
    }

    void build() {
        for (int i = 1; i <= n; ++i) {
            if (vis[i]) continue;
            depth[i] = 1;
            dfs(i, 0);
            if (!st.empty())
                saturate();
        }

        int m = Components.size();
        int nextId = m;
        tree.assign(m + 1, {});

        for (int u = 1; u <= m; ++u)
            for (int v: Components[u - 1])
                if (id[v] == 0) id[v] = u;

        for (int u = 1; u <= m; ++u) {
            for (int v: Components[u - 1]) {
                if (!is_art[v]) continue;
                if (id[v] <= m) {
                    id[v] = ++nextId;
                    if (tree.size() <= id[v]) tree.resize(id[v] + 1);
                }
                tree[u].emplace_back(id[v]);
                tree[id[v]].emplace_back(u);
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

    Articulation T(n, adj);
    LCA lc(T.tree);

    while (q--) {
        int a, b, c;
        cin >> a >> b >> c;
        if (a == c || b == c) {
            cout << "NO\n";
            continue;
        }
        if (T.is_art[c] == 0) {
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