#include <bits/stdc++.h>

using namespace std;
#define int long long

struct SCC {
    vector<vector<int>> adj, r_adj, scc, dag;
    vector<int> id, vis, order, comp;
    int n;

    SCC(int n, vector<vector<int>> &adj, vector<vector<int>> &r_adj) :
            adj(adj), r_adj(r_adj), n(n), id(n + 1), vis(n + 1), dag(n + 1) {
        build();
    };

    void dfs(int u, vector<vector<int>> &_adj) {
        vis[u] = 1;
        for (auto v: _adj[u])
            if (!vis[v])dfs(v, _adj);

        comp.emplace_back(u);
    }

    void build() {
        for (int i = 1; i <= n; ++i)
            if (!vis[i]) dfs(i, adj);

        order = comp;
        reverse(order.begin(), order.end());
        vis.assign(n + 1, 0);
        for (auto u: order) {
            if (vis[u])continue;
            comp.clear();
            dfs(u, r_adj);
            scc.emplace_back(comp);
            for (auto v: comp)
                id[v] = scc.size();
        }

        for (int u = 1; u <= n; ++u) {
            for (auto v: adj[u]) {
                if (id[v] != id[u])
                    dag[id[u]].emplace_back(id[v]);
            }
        }
    }

    vector<int> topological() {
        int m = scc.size();
        vector<int> in(m + 1), topo;
        for (int u = 1; u <= m; ++u)
            for (auto v: dag[u])
                in[v]++;
        queue<int> q;
        for (int i = 1; i <= m; ++i)
            if (in[i] == 0)
                q.emplace(i), topo.emplace_back(i);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto v: dag[u])
                if (--in[v] == 0) q.push(v), topo.emplace_back(v);
        }
        return topo;
    }
};


void solve() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n + 1), r_adj(n + 1);
    for (int i = 0, u, v; i < m; ++i) {
        cin >> u >> v;
        adj[u].emplace_back(v);
        r_adj[v].emplace_back(u);
    }
    SCC G(n, adj, r_adj);
    cout << G.scc.size() << '\n';
    for (int i = 1; i <= n; ++i)
        cout << G.id[i] << ' ';
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