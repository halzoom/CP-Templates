#include<bits/stdc++.h>

using namespace std;
#define int long long
static const int inf = 1e18;

// O(V^4) for worst case because of spfa
// O(V^2 * E)
struct Edge {
    int u, v, flow = 0, cap = 0, cost; // keep the order
    Edge(int u, int v, int c, int cost) : u(u), v(v), cap(c), cost(cost) {}

    int rem() { return cap - flow; }
};

struct MCMF {
    int n, s, t, cost = 0, flow = 0;
    vector<Edge> edges;
    vector<vector<int>> adj;
    vector<int> from;

    MCMF(int n, int s, int t) : n(n), s(s), t(t) {
        adj.assign(n + 1, {});
    }

    void addEdge(int u, int v, int w = inf, int cost = 0, int undir = 0) {
        adj[u].push_back(edges.size());
        edges.push_back(Edge(u, v, w, cost));
        adj[v].push_back(edges.size());
        edges.push_back(Edge(v, u, w * undir, -cost));
    }

    int max_flow() {
        while (bfs()) {
            int u = t, addflow = inf;
            while (u != s) {
                Edge &e = edges[from[u]];
                addflow = min(addflow, e.rem());
                u = e.u;
            }

            u = t;
            while (u != s) {
                int i = from[u];
                edges[i].flow += addflow;
                edges[i ^ 1].flow -= addflow;
                cost += edges[i].cost * addflow;
                u = edges[i].u;
            }

            flow += addflow;
        }
        return flow;
    }

    bool bfs() {
        from.assign(n + 1, -1);
        vector<int> d(n + 1, inf), state(n + 1, 2);
        deque<int> q;
        state[s] = 1, d[s] = 0;
        q.clear();
        q.push_back(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop_front();
            state[u] = 0;
            for (auto &i: adj[u]) {
                auto &[_, v, f, c, cost] = edges[i];
                if (f >= c || d[v] <= d[u] + cost) continue;

                d[v] = d[u] + cost;

                from[v] = i;
                if (state[v] == 1) continue;
                if (!state[v] || (!q.empty() && d[q.front()] > d[v]))
                    q.push_front(v);
                else q.push_back(v);
                state[v] = 1;
            }
        }

        return ~from[t];
    }
};

vector<vector<int>> getRoutes(MCMF &T, int source, int sink, int k) {
    vector<vector<int>> routes;
    for (int i = 0; i < k; ++i) {
        vector<int> path;
        int u = source;
        while (u != sink) {
            path.push_back(u);
            bool moved = false;

            for (int idx: T.adj[u]) {
                if (idx & 1 or T.edges[idx].flow == 0)continue;
                T.edges[idx].flow--;
                u = T.edges[idx].v;
                moved = true;
                break;
            }
            if (!moved) break;
        }
        path.push_back(sink);
        routes.push_back(path);
    }
    return routes;
}

void solve() {
    int n;
    cin >> n;
    int src = 0, sink = 2 * n + 1;
    MCMF T(2 * n + 2, src, sink);
    for (int i = 1; i <= n; ++i) {
        for (int j = 1, w; j <= n; ++j) {
            cin >> w;
            T.addEdge(i, j + n, 1, w);
        }
    }
    for (int i = 1; i <= n; ++i)
        T.addEdge(src, i, 1);
    for (int i = 1; i <= n; ++i)
        T.addEdge(i + n, sink, 1);

    T.max_flow();
    cout << T.cost << '\n';
    for (int i = 0; i < T.edges.size(); i += 2) {
        if (T.edges[i].flow != 1 or T.edges[i].v <= n)continue;
        if (T.edges[i].v == sink)continue;
        cout << T.edges[i].u << ' ' << T.edges[i].v - n << '\n';
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