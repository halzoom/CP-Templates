#include <bits/stdc++.h>

using namespace std;
#define int long long

struct SCC {
    vector<vector<int>> adj, r_adj, scc, dag;
    vector<int> id, vis, order, comp;
    int n;

    SCC(int n, vector<vector<int>> &adj, vector<vector<int>> &r_adj) :
            adj(adj), r_adj(r_adj), n(n), id(n + 1), vis(n + 1) {
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

        dag.assign(scc.size() + 1, {});
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


// return node in the dag note in the actual adj for SCC
vector<pair<int, int>> DAG_to_SCC(vector<vector<int>> &dag) {
    int n = dag.size() - 1;
    if (n <= 1) return {};

    vector<int> vis(n + 1), in(n + 1), out(n + 1);
    for (int u = 1; u <= n; ++u)
        for (auto v: dag[u])
            in[v]++, out[u]++;

    function<int(int)> dfs = [&](int u) -> int {
        if (vis[u]) return -1;
        vis[u] = 1;
        if (out[u] == 0) return u;
        for (int v: dag[u]) {
            int r = dfs(v);
            if (r != -1) return r;
        }
        return -1;
    };

    vector<int> matched_sources, matched_sinks;
    vector<int> never_matched_sources, never_matched_sinks;

    for (int i = 1; i <= n; ++i) {
        if (in[i])continue;
        int sink = dfs(i);
        if (sink != -1) {
            matched_sources.emplace_back(i);
            matched_sinks.emplace_back(sink);
        } else {
            never_matched_sources.emplace_back(i);
        }
    }

    for (int i = 1; i <= n; ++i)
        if (out[i] == 0 && !vis[i])
            never_matched_sinks.emplace_back(i);

    vector<int> sources = matched_sources;
    for (auto u: never_matched_sources)
        sources.emplace_back(u);
    vector<int> sinks = matched_sinks;
    for (auto u: never_matched_sinks)
        sinks.emplace_back(u);

    int P = matched_sources.size(), N = sources.size();
    int M = sinks.size(), K = min(N, M);
    vector<pair<int, int>> idxPairs;
    for (int i = 0; i < P; ++i) idxPairs.emplace_back(i, (i - 1 + P) % P);
    for (int i = P; i < K; ++i) idxPairs.emplace_back(i, i);
    for (int i = K; i < N; ++i) idxPairs.emplace_back(0, i);
    for (int i = K; i < M; ++i) idxPairs.emplace_back(i, 0);

    vector<pair<int, int>> answer;
    answer.reserve(idxPairs.size());
    for (auto [i, j]: idxPairs)
        answer.emplace_back(sinks[i], sources[j]);

    return answer;
}

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