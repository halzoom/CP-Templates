#include <bits/stdc++.h>

using namespace std;
static const int inf = 1e9;

// O(V^4) for worst case because of spfa
// O(V^2 * E)
struct MCMF {
    struct Edge {
        int to, cap, flow, cost, rev;
    };
    vector<vector<Edge>> adj;
    vector<long long> h, dist;
    vector<int> p_v, p_e;

    MCMF(int n) : adj(n), h(n), dist(n), p_v(n), p_e(n) {}

    void addEdge(int u, int v, int cap, int cost = 0) {
        adj[u].push_back({v, cap, 0, cost, (int) adj[v].size()});
        adj[v].push_back({u, 0, 0, -cost, (int) adj[u].size() - 1});
    }

    pair<int, int> min_cost_max_flow(int s, int t, int f) {
        int res = 0;
        fill(h.begin(), h.end(), 0);
        while (f > 0) {
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> que;
            fill(dist.begin(), dist.end(), inf);
            dist[s] = 0;
            que.push({0, s});
            while (!que.empty()) {
                pair<int, int> p = que.top();
                que.pop();
                int v = p.second;
                if (dist[v] < p.first) continue;
                for (int i = 0; i < adj[v].size(); i++) {
                    Edge &e = adj[v][i];
                    if (e.cap > e.flow && dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]) {
                        dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
                        p_v[e.to] = v;
                        p_e[e.to] = i;
                        que.push({dist[e.to], e.to});
                    }
                }
            }
            if (dist[t] == inf) break;
            for (int v = 0; v < h.size(); v++) h[v] += dist[v];

            int d = f;
            for (int v = t; v != s; v = p_v[v]) {
                d = min(d, adj[p_v[v]][p_e[v]].cap - adj[p_v[v]][p_e[v]].flow);
            }
            f -= d;
            res += d * h[t];
            for (int v = t; v != s; v = p_v[v]) {
                Edge &e = adj[p_v[v]][p_e[v]];
                e.flow += d;
                adj[v][e.rev].flow -= d;
            }
        }
        return {f, res};
    }
};

void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> a[i];

    int src = 0, src1 = n * 2 + 2, sink = 2 * n + 1;
    MCMF T(2 * n + 3);
    T.addEdge(src, src1, 4);
    for (int i = 1; i <= n; ++i) {
        T.addEdge(src1, i, 1, 0);
        T.addEdge(i, i + n, 1, -1);
        T.addEdge(i + n, sink, 1, 0);

        for (int j = i + 1; j <= n; ++j) {
            if (abs(a[i] - a[j]) != 1 and a[i] % 7 != a[j] % 7)continue;
            T.addEdge(i + n, j, 1, 0);
        }
    }
    cout << -T.min_cost_max_flow(src, sink, 1e8).second << '\n';
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