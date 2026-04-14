#include <bits/stdc++.h>

using namespace std;
#define int long long
const int inf = 1e18;

struct FloydWarshall {
    vector<vector<int>> dist, next;
    int n;

    FloydWarshall(int n_, vector<array<int, 3>> edge) {
        n = n_;
        dist.assign(n + 1, vector<int>(n + 1, inf));
        next.assign(n + 1, vector<int>(n + 1));
        for (auto [u, v, w]: edge) {
            dist[u][v] = min(dist[u][v], w);
            next[u][v] = v;
        }
        for (int i = 1; i <= n; ++i) {
            dist[i][i] = 0;
            next[i][i] = i;
        }

        for (int k = 1; k <= n; ++k) {
            for (int u = 1; u <= n; ++u) {
                for (int v = 1; v <= n; ++v) {
                    if (dist[u][k] == inf or dist[k][v] == inf)continue;
                    if (dist[u][k] + dist[k][v] >= dist[u][v])continue;
                    dist[u][v] = dist[u][k] + dist[k][v];
                    next[u][v] = next[u][k];
                }
            }
        }

        for (int k = 1; k <= n; k++) {
            if (dist[k][k] >= 0)continue;
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= n; j++) {
                    if (dist[i][k] < inf and dist[k][j] < inf) {
                        dist[i][j] = -inf;
                    }
                }
            }
        }
    }

    vector<int> get_path(int u, int v) {
        vector<int> path;
        if (!next[u][v])return path;
        path.emplace_back(u);
        while (u != v) {
            u = next[u][v];
            path.emplace_back(u);
        }
        return path;
    }

    vector<int> negative_cycle() {
        vector<int> cycle;
        for (int i = 1; i <= n; i++) {
            if (dist[i][i] >= 0)continue;
            int x = i;
            for (int k = 1; k <= n; k++) x = next[x][i];

            int cur = x;
            do {
                cycle.push_back(cur);
                cur = next[cur][i];
            } while (cur != x and cur);
            break;
        }
        return cycle;
    }
};

void solve() {
    int n, m, q;
    cin >> n >> m >> q;
    vector<array<int, 3>> edge;
    for (int i = 0, u, v, w; i < m; ++i) {
        cin >> u >> v >> w;
        edge.push_back({u, v, w});
        edge.push_back({v, u, w});
    }

    FloydWarshall T(n, edge);
    while (q--) {
        int u, v;
        cin >> u >> v;
        cout << (T.dist[u][v] >= inf ? -1 : T.dist[v][u]) << "\n";
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