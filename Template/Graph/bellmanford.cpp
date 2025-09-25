#include <bits/stdc++.h>

using namespace std;
#define int long long
const int inf = 1e18;

struct BellmanFord {
    vector<int> dist, parent;
    int has_negative_cycle = -1, n;

    BellmanFord(int s, int _n, vector<array<int, 3>> &edge) {
        n = _n;
        parent.assign(n + 1, -1);
        dist.assign(n + 1, inf);
        dist[s] = 0;

        for (int i = 1; i <= n; ++i) {
            has_negative_cycle = -1;
            for (auto [u, v, w]: edge) {
                if (dist[u] == inf) continue;
                if (dist[v] > dist[u] + w) {
                    dist[v] = max(-inf, dist[u] + w);
                    parent[v] = u;
                    has_negative_cycle = v;
                }
            }
        }

        if (has_negative_cycle != -1) {
            for (int i = 1; i <= n; i++) {
                for (auto [u, v, w]: edge) {
                    if (dist[u] == -inf) dist[v] = -inf;
                    else if (dist[u] != inf && dist[v] > dist[u] + w) {
                        dist[v] = -inf;
                    }
                }
            }
        }
    };

    vector<int> negative_cycle() {
        if (has_negative_cycle == -1)
            return {-1};
        int y = has_negative_cycle;
        for (int i = 1; i < n; ++i)
            y = parent[y];

        vector<int> path;
        for (int cur = y;; cur = parent[cur]) {
            path.push_back(cur);
            if (cur == y && path.size() > 1)
                break;
        }
        reverse(path.begin(), path.end());
        return path;
    }
};

void solve() {
    int n, m, q, s;
    while (true) {
        cin >> n >> m >> q >> s;
        if (!n)break;
        s++;
        vector<array<int, 3>> edge(m);
        for (int i = 0, u, v, w; i < m; ++i) {
            cin >> u >> v >> w;
            edge[i] = {u + 1, v + 1, w};
        }

        BellmanFord T(s, n, edge);
        auto D = T.dist;
        while (q--) {
            int u;
            cin >> u, ++u;
            if (D[u] >= inf)
                cout << "Impossible\n";
            else if (D[u] <= -inf)
                cout << "-Infinity\n";
            else
                cout << D[u] << '\n';
        }
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