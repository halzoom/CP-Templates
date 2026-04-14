#include <bits/stdc++.h>

using namespace std;
#define int long long
// O(V^2 * E)
// O(E * Sqrt(V)) in maximum matching problem (Unit Networks)

static const int inf = 2e15;

struct Edge {
    int u, v, flow = 0, cap = 0; // keep the order

    Edge(int u, int v) : u(u), v(v) {}

    Edge(int u, int v, int c) : u(u), v(v), cap(c) {}

    int rem() { return cap - flow; }
};

struct Dinic {
    int n, s, t, id = 1, flow = 0;
    vector<Edge> edges;
    vector<vector<int>> adj;
    vector<int> lvl, ptr;

    Dinic(int n, int src, int sink) : n(n), s(src), t(sink) {
        adj.assign(n + 1, {});
        ptr.assign(n + 1, {});
    }

    void addEdge(int u, int v, int w = inf, int undir = 0) {
        adj[u].push_back(edges.size());
        edges.push_back(Edge(u, v, w));
        adj[v].push_back(edges.size());
        edges.push_back(Edge(v, u, w * undir));
    }

    int max_flow() {
        while (bfs()) {
            ptr.assign(n + 1, {});
            while (int f = dfs(s)) flow += f;
        }
        return flow;
    }

    bool bfs() {
        lvl.assign(n + 1, -1);
        queue<int> q;
        q.push(s), lvl[s] = 0;

        while (!q.empty()) {
            auto u = q.front();
            q.pop();

            for (auto &i: adj[u]) {
                auto &[_, v, f, c] = edges[i];
                if (~lvl[v] || f == c) continue;
                lvl[v] = lvl[u] + 1;
                q.push(v);
            }
        }

        return lvl[t] != -1;
    }

    int dfs(int u, int currFlow = inf) {
        if (u == t) return currFlow;
        if (!currFlow) return 0;

        for (; ptr[u] < adj[u].size(); ++ptr[u]) {
            int i = adj[u][ptr[u]];
            auto [_, v, f, c] = edges[i];
            if (f == c || (lvl[v] != lvl[u] + 1)) continue;

            int bottleNeck = dfs(v, min(currFlow, c - f));
            if (!bottleNeck) continue;

            edges[i].flow += bottleNeck;
            edges[i ^ 1].flow -= bottleNeck;

            return bottleNeck;
        }
        return 0;
    }

    vector<array<int, 2>> getEdges() {
        vector<int> srcSide(n + 1);
        queue<int> q;
        q.push(s), srcSide[s] = 1;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &i: adj[u]) {
                auto &[_, v, f, c] = edges[i];
                if (!srcSide[v] && f != c) q.push(v), srcSide[v] = 1;
            }
        }

        vector<array<int, 2>> res;
        for (int i = 0; i < edges.size(); i += 2) {
            auto &[u, v, f, c] = edges[i];
            if (srcSide[u] != srcSide[v] && c != inf) res.push_back({u, v});
        }
        return res;
    }
};

void solve() {
    int n, m;
    cin >> n >> m;
    Dinic T(n, 1, n);
    for (int i = 0, u, v; i < m; ++i) {
        cin >> u >> v;
        T.addEdge(u, v, 1, 1);
    }
    cout << T.max_flow() << '\n';
    auto bad = T.getEdges();
    for (auto [u, v]: bad)
        cout << u << ' ' << v << '\n';
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