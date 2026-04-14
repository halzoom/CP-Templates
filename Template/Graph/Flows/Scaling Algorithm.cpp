#include <bits/stdc++.h>

using namespace std;
#define int long long
const int inf = 1e18;
// Scaling algorithm for computing the maximum flow in a flow network.
// Runs in O(m^2 log C) time.
// Initially the threshold value C is chosen as a large number, for example, the maximum edge weight of the graph.
struct Edge {
    int from, to, w;
};

struct MaxFlow {
    int n, source, sink;
    vector<vector<int>> adj, paths;
    vector<Edge> edges;
    vector<bool> seen;
    int flow = 0;

    MaxFlow(int n, int source, int sink)
            : n(n + 1), source(source), sink(sink), adj(n + 1) {}

    int addEdge(int from, int to, int forward, int backward = 0) {
        const int id = (int) edges.size();
        adj[from].emplace_back(id);
        edges.push_back({from, to, forward});
        adj[to].emplace_back(id + 1);
        edges.push_back({to, from, backward});
        return id;
    }

    vector<int> path;

    bool dfs(int node, int lim) {
        if (node == sink) return true;
        if (seen[node]) return false;
        path.emplace_back(node);
        seen[node] = true;
        for (int i: adj[node]) {
            auto &e = edges[i];
            auto &back = edges[i ^ 1];
            if (e.w >= lim) {
                if (dfs(e.to, lim)) {
                    e.w -= lim;
                    back.w += lim;
                    return true;
                }
            }
        }
        path.pop_back();
        return false;
    }

    int max_flow() {
        for (int bit = 1ll << 62; bit > 0; bit /= 2) { // update this to max edge weight
            bool found = false;
            do {
                seen.assign(n, false);
                path.clear();
                found = dfs(source, bit);
                flow += bit * found;
                if (found)
                    paths.emplace_back(path);
            } while (found);
        }
        return flow;
    }
};

void solve() {
    int n, m;
    cin >> n >> m;
    MaxFlow T(n, 1, n);
    for (int i = 0, u, v; i < m; ++i) {
        cin >> u >> v;
        T.addEdge(u, v, 1);
    }

    vector<int> path;
    function<void(int)> get = [&](int u) {
        path.emplace_back(u);
        if (u == n)return;
        for (auto i: T.adj[u]) {
            auto &[from, to, w] = T.edges[i];
            if (w or i & 1)continue;
            w = 1;
            get(to);
            return;
        }
    };
    cout << T.max_flow() << '\n';
    for (int i = 0; i < T.flow; ++i) {
        path.clear();
        get(1);
        cout << path.size() << '\n';
        for (auto u: path)
            cout << u << " ";
        cout << '\n';
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