#include <bits/stdc++.h>

using namespace std;
#define int long long

struct SCC {
    int n, timer = 1, sz = 0;
    vector<vector<int>> adj, comp, dag;
    vector<int> in, low, id, st, inStack;

    SCC(int _n, vector<vector<int>> &_adj) {
        n = _n, sz = 0, timer = 1;
        adj = _adj;;
        build();
    };

    void build() {
        in.assign(n + 1, 0);
        low.assign(n + 1, 0);
        id.assign(n + 1, 0);
        inStack.assign(n + 1, 0);

        for (int u = 1; u <= n; ++u)
            if (!in[u]) dfs(u);

        getDAG();
    }

    void dfs(int u) {
        in[u] = low[u] = timer++;
        inStack[u] = 1;
        st.push_back(u);

        for (int &v: adj[u]) {
            if (!in[v]) dfs(v), low[u] = min(low[u], low[v]);
            else if (inStack[v]) low[u] = min(low[u], in[v]);
        }

        if (low[u] == in[u]) {
            comp.emplace_back();
            int v = -1;
            while (v != u) {
                v = st.back(), st.pop_back(), inStack[v] = 0;
                id[v] = sz + 1;
                comp.back().push_back(v);
            }
            ++sz;
        }
    }

    void getDAG() {
        // BE CAREFUL OF MUTIPLE EDGES
        dag.assign(sz + 1, {});
        for (int u = 1; u <= n; ++u)
            for (auto v: adj[u])
                if (id[u] != id[v])
                    dag[id[u]].push_back(id[v]);
    }
};

void solve() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    for (int i = 0, u, v; i < m; ++i) {
        cin >> u >> v;
        adj[u].emplace_back(v);
    }

    SCC T(n, adj);

    cout << T.sz << '\n';
    for (int i = 1; i <= n; ++i)
        cout << T.id[i] << ' ';
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