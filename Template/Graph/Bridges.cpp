#include <bits/stdc++.h>

using namespace std;
#define int long long
const int inf = 1e18;

struct Bridges {
    vector<vector<int>> adj, tree, Components;
    vector<int> low, vis, depth, stack, id;
    int n;
    vector<pair<int, int>> bridge;

    Bridges(int n, vector<vector<int>> &adj) :
            adj(adj), n(n), depth(n + 1), id(n + 1), vis(n + 1), low(n + 1, inf) {
        build();
    };

    void dfs(int u, int p) {
        stack.emplace_back(u);
        low[u] = depth[u];
        vis[u] = 1;
        for (auto v: adj[u]) {
            if (v == p)continue;
            if (vis[v]) {
                low[u] = min(low[u], depth[v]);
                continue;
            }
            depth[v] = depth[u] + 1;
            dfs(v, u);
            low[u] = min(low[u], low[v]);
            if (low[v] > depth[u])
                bridge.emplace_back(u, v);
        }
        if (low[u] == depth[u]) {
            vector<int> comp;
            while (stack.back() != u)
                comp.emplace_back(stack.back()), stack.pop_back();
            comp.emplace_back(stack.back());
            stack.pop_back();
            Components.emplace_back(comp);
            for (auto v: comp)
                id[v] = Components.size();
        }
    }

    void build() {
        for (int i = 1; i <= n; ++i)
            if (!vis[i])
                dfs(i, 0);

        tree.assign(Components.size() + 1, {});
        for (int u = 1; u <= n; ++u) {
            for (auto v: adj[u]) {
                if (id[u] != id[v])
                    tree[id[u]].emplace_back(id[v]);
            }
        }
    }
};

struct LCA {
    vector<vector<int>> ancestor;
    vector<int> level;
    int LG;

    LCA(vector<vector<int>> &adj) {
        int n = (int) adj.size();
        LG = __lg(n) + 1;
        ancestor.assign(LG, vector<int>(n));
        level.assign(n, {});
        build(1, 0, adj);
        for (int i = 1; i < LG; ++i)
            for (int u = 1; u < n; ++u)
                ancestor[i][u] = ancestor[i - 1][ancestor[i - 1][u]];

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
};

void solve() {

    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    for (int i = 1, u, v; i <= m; ++i) {
        cin >> u >> v;
        adj[u].emplace_back(v);
        adj[v].emplace_back(u);
    }

    Bridges G(n, adj);
    LCA lc(G.tree);
    int q;
    cin >> q;
    while (q--) {
        int u, v;
        cin >> u >> v;
        cout << lc.getDist(G.id[u], G.id[v]) << '\n';
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