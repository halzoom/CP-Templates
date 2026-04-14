#include <bits/stdc++.h>

using namespace std;
#define int long long
const int inf = 1e18;

struct SPFA {
    vector<long long> dist;
    vector<int> parent, in_queue, relax_count;
    int n;
    int has_negative_cycle = -1;

    SPFA(int s, int _n, vector<vector<pair<int,int>>> &adj) {
        n = _n;
        dist.assign(n + 1, inf);
        parent.assign(n + 1, -1);
        in_queue.assign(n + 1, 0);
        relax_count.assign(n + 1, 0);

        queue<int> q;
        dist[s] = 0;
        q.push(s);
        in_queue[s] = 1;

        bool neg_cycle_found = false;

        while (!q.empty() && !neg_cycle_found) {
            int u = q.front(); q.pop();
            in_queue[u] = 0;

            for (auto [v, w] : adj[u]) {
                if (dist[u] == inf) continue;

                if (dist[v] > dist[u] + w) {
                    dist[v] = max(-inf, dist[u] + w);
                    parent[v] = u;

                    if (!in_queue[v]) {
                        q.push(v);
                        in_queue[v] = 1;
                        relax_count[v]++;

                        if (relax_count[v] >= n) {
                            has_negative_cycle = v;
                            neg_cycle_found = true;
                            break;
                        }
                    }
                }
            }
        }

        if (has_negative_cycle != -1) {
            queue<int> qq;
            vector<int> vis(n + 1, 0);
            qq.push(has_negative_cycle);
            vis[has_negative_cycle] = 1;
            while (!qq.empty()) {
                int u = qq.front(); qq.pop();
                dist[u] = -inf;
                for (auto [v, w] : adj[u]) {
                    if (!vis[v]) {
                        qq.push(v);
                        vis[v] = 1;
                    }
                }
            }
        }
    }

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
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int>>> adj(n + 1);
    for (int i = 0, u, v, w; i < m; ++i) {
        cin >> u >> v >> w;
        adj[u].emplace_back(v, w);
    }
    optional<int>x;

    for (int i = 1; i <= n; ++i) {
        SPFA T(i, n, adj);

        auto cycle = T.negative_cycle();
        if (cycle.back() == -1)continue;
        cout << "YES\n";
        for (auto v: cycle)
            cout << v << ' ';
        return;
    }

    cout << "NO\n";

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