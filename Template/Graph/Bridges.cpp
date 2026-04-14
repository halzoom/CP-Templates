#include <bits/stdc++.h>

using namespace std;
#define int long long

struct Bridges {
    int n, timer = 1, sz = 0;
    vector<vector<int>> adj, tree, BCC;
    vector<int> in, low, st, root;
    vector<pair<int, int>> bridge;

    Bridges(int _n, vector<vector<int>> &_adj) {
        n = _n;
        adj = _adj;
        build(adj);
    };

    void build(auto &_adj) {
        adj = _adj;
        in.assign(n + 1, 0);
        low.assign(n + 1, 0);
        root.assign(n + 1, 0);

        for (int u = 1; u <= n; ++u)
            if (!in[u]) dfs(u);

        sz = BCC.size();
        tree.assign(n + 1, {});
        for (int u = 1; u <= n; u++)
            for (int v: adj[u])
                if (root[u] != root[v])
                    tree[root[u]].push_back(root[v]);
    }

    void dfs(int u, int p = 0) {
        st.push_back(u);
        in[u] = low[u] = timer++;
        bool pFound = false; // for multiple edges
        for (int &v: adj[u]) {
            if (!pFound and v == p) {
                pFound = true;
                continue;
            }

            if (!in[v]) dfs(v, u);
            low[u] = min(low[u], low[v]);
            if (low[v] > in[u]) bridge.emplace_back(u, v);
        }

        if (low[u] == in[u]) {
            vector<int> c;
            while (st.back() != u)
                c.push_back(st.back()), st.pop_back();
            c.push_back(st.back()), st.pop_back();
            for (int v: c) root[v] = c.front();
            BCC.push_back(c);
        }
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

    Bridges T(n, adj);
    if (T.bridge.size())
        return void(cout << "0\n");

    vector<int> vis(n + 1), depth(n + 1);
    function<void(int, int)> go = [&](int u, int p) {
        vis[u] = 1;
        for (auto v: adj[u]) {
            if (v == p)continue;
            if (!vis[v]) {
                cout << u << ' ' << v << '\n';
                depth[v] = depth[u] + 1;
                go(v, u);
            } else if (depth[v] < depth[u])
                cout << u << ' ' << v << '\n';
        }
    };

    go(1, 0);

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