#include <bits/stdc++.h>

using namespace std;
#define int long long
static const int inf = 1e15;


struct Edge {
    int u, v, flow = 0, cap = 0, cost = 0;  // all ll now
    Edge(int u, int v, int c, int cost) : u(u), v(v), cap(c), cost(cost) {}

    int rem() const { return cap - flow; }
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

    void move() {
        while (bfs()) {
            int addflow = inf;
            for (int u = t; u != s; u = edges[from[u]].u)
                addflow = min(addflow, edges[from[u]].rem());

            for (int u = t; u != s; u = edges[from[u]].u) {
                int i = from[u];
                edges[i].flow += addflow;
                edges[i ^ 1].flow -= addflow;
                cost += edges[i].cost * addflow;
            }
            flow += addflow;
        }
    }

    bool bfs() {
        from.assign(n + 1, -1);
        vector<int> d(n + 1, inf);
        vector<int> state(n + 1, 2);
        deque<int> q;

        d[s] = 0;
        state[s] = 1;
        q.push_back(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop_front();
            state[u] = 0;
            for (auto &ei: adj[u]) {
                auto &e = edges[ei];
                int v = e.v;
                if (e.rem() <= 0 || d[v] <= d[u] + e.cost) continue;
                d[v] = d[u] + e.cost;
                from[v] = ei;
                if (state[v] == 1) continue;
                if (state[v] == 0 || (!q.empty() && d[q.front()] > d[v]))
                    q.push_front(v);
                else
                    q.push_back(v);
                state[v] = 1;
            }
        }
        return from[t] != -1;
    }
};

void solve() {
    int n;
    cin >> n;
    vector<array<int, 3>> edge(n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < 3; ++j)
            cin >> edge[i][j];

    int src1 = 0, src = 303, sink = 301, shift = 150;
    MCMF T(305, src, sink);
    for (auto [u, v, w]: edge)
        T.addEdge(u, v + shift, 1, -w);

    for (int i = 1; i <= shift; ++i)
        T.addEdge(src1, i, 1, 0);
    for (int i = shift + 1; i < sink; ++i)
        T.addEdge(i, sink, 1, 0);


    vector<int> answer;
    for (int k = 1; k <= 150; ++k) {
        T.addEdge(src, src1, 1, 0);
        T.move();
        if (T.flow != k)break;
        answer.emplace_back(-T.cost);
    }
    cout << answer.size() << '\n';
    for (auto i: answer)
        cout << i << '\n';
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